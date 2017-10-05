/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiServer_Server.h"

#include <flip/BackEndBinary.h>

#include <flip/contrib/DataProviderFile.h>
#include <flip/contrib/DataConsumerFile.h>
#include <flip/contrib/RunLoopTimer.h>

#include <KiwiModel/KiwiModel_DataModel.h>

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                      SERVER                                      //
        // ================================================================================ //
        
        std::string hexadecimal_convert(uint64_t hexa_decimal)
        {
            std::stringstream converter;
            converter << std::hex << hexa_decimal;
            
            return converter.str();
        }
        
        const char* Server::kiwi_file_extension = "kiwi";
        
        Server::Server(uint16_t port, std::string const& backend_directory) :
        m_running(false),
        m_backend_directory(backend_directory),
        m_sessions(),
        m_socket(*this, port),
        m_ports()
        {
            if (m_backend_directory.exists() && !m_backend_directory.isDirectory())
            {
                throw std::runtime_error("Specified backend directory is a file");
            }
            else if(!m_backend_directory.exists())
            {
                m_backend_directory.createDirectory();
            }
        }
        
        Server::~Server()
        {
            m_socket.reset();
        }
        
        void Server::process()
        {
            m_socket.process();
        }
        
        void Server::run()
        {
            if(!m_running)
            {
                flip::RunLoopTimer run_loop ([this]
                {
                    process();
                    return m_running.load();
                }, 0.02);
                
                m_running.store(true);
                
                run_loop.run();
            }
        }
        
        void Server::stop()
        {
            m_running.store(false);
            std::cout << "[server] - stopped" << std::endl;
        }
        
        bool Server::isRunning() const noexcept
        {
            return m_running;
        }
        
        std::set<uint64_t> Server::getSessions() const
        {
            std::set<uint64_t> sessions;
            
            for(auto & session : m_sessions)
            {
                sessions.insert(session.first);
            }
            
            return sessions;
        }
        
        std::set<uint64_t> Server::getConnectedUsers(uint64_t session_id) const
        {
            auto session = m_sessions.find(session_id);
            return session != m_sessions.end() ? session->second.getConnectedUsers() : std::set<uint64_t>();
        }
        
        juce::File Server::getSessionFile(uint64_t session_id) const
        {
            return m_backend_directory.getChildFile(juce::String(hexadecimal_convert(session_id)))
            .withFileExtension(kiwi_file_extension);
        }
        
        void Server::createSession(uint64_t session_id)
        {
            const auto session_file = getSessionFile(session_id);
            
            m_sessions.insert(std::make_pair(session_id, Session(session_id, session_file)));
        }
        
        void Server::onConnected(flip::PortBase & port)
        {
            if(m_sessions.find(port.session()) == m_sessions.end())
            {
                createSession(port.session());
            }
            
            m_sessions.find(port.session())->second.bind(port);
        }
        
        void Server::onDisconnected(flip::PortBase & port)
        {
            port.impl_activate(false);
            
            auto session = m_sessions.find(port.session());
            
            session->second.unbind(port);
            
            if (session->second.getConnectedUsers().empty())
            {
                m_sessions.erase(session);
            }
        }
        
        void Server::port_factory_add(flip::PortBase& port)
        {
            port.impl_bind (*this);
            
            m_ports.insert (&port);
        }
        
        void Server::port_factory_remove(flip::PortBase& port)
        {
            if (m_ports.erase (&port)) {port.impl_unbind (*this);}
            
            onDisconnected(port);
        }
        
        void Server::port_greet(flip::PortBase& port)
        {
            if (m_ports.erase (&port)) { port.impl_unbind (*this); }
            
            onConnected(port);
        }
        
        void Server::port_commit(flip::PortBase& /* port */, const flip::Transaction& /* tx */)
        {
            // nothing
        }
        
        void Server::port_squash(flip::PortBase& /* port */, const flip::TxIdRange& /* range */, const flip::Transaction& /* tx */)
        {
            // nothing
        }
        
        void Server::port_push(flip::PortBase& /* port */, const flip::Transaction& /* tx */)
        {
            // port was rebound in greet
            flip_FATAL;
        }
        
        void Server::port_signal(flip::PortBase& /* port */, const flip::SignalData& /* data */)
        {
            // port was rebound in greet
            flip_FATAL;
        }
        
        // ================================================================================ //
        //                                   SERVER SESSION                                 //
        // ================================================================================ //
        
        Server::Session::Session(Session && other)
        : m_identifier(std::move(other.m_identifier))
        , m_validator(std::move(other.m_validator))
        , m_document(std::move(other.m_document))
        , m_signal_connections(std::move(other.m_signal_connections))
        , m_backend_file(std::move(other.m_backend_file))
        {
            ;
        }
        
        Server::Session::Session(uint64_t identifier, juce::File const& backend_file)
        : m_identifier(identifier)
        , m_validator(new model::PatcherValidator())
        , m_document(new flip::DocumentServer(model::DataModel::use(), *m_validator, m_identifier))
        , m_signal_connections()
        , m_backend_file(backend_file)
        {
            DBG("[server] - creating new session for session_id : " << hexadecimal_convert(m_identifier));
            
            model::Patcher& patcher = m_document->root<model::Patcher>();
            
            auto cnx = patcher.signal_get_connected_users.connect(std::bind(&Server::Session::sendConnectedUsers, this));
            
            m_signal_connections.emplace_back(std::move(cnx));
            
            if (m_backend_file.exists())
            {
                DBG("[server] - loading session file for session_id : " << hexadecimal_convert(m_identifier));
                
                load();
            }
            else
            {
                DBG("[server] - initializing with empty document for session_id : " << hexadecimal_convert(m_identifier));
            }
        }
        
        Server::Session::~Session()
        {
            if (m_document)
            {
                std::set<flip::PortBase*> ports = m_document->ports();
                
                for (std::set<flip::PortBase*>::iterator port = ports.begin(); port != ports.end(); ++port)
                {
                    m_document->port_factory_remove(**port);
                }
            }
        }
        
        uint64_t Server::Session::getId() const
        {
            return m_identifier;
        }
        
        void Server::Session::save() const
        {
            flip::BackEndIR backend(m_document->write());
            
            flip::DataConsumerFile consumer(m_backend_file.getFullPathName().toStdString().c_str());
            
            backend.write<flip::BackEndBinary>(consumer);
        }
        
        void Server::Session::load()
        {
            flip::BackEndIR backend;
            backend.register_backend<flip::BackEndBinary>();
            
            flip::DataProviderFile provider(m_backend_file.getFullPathName().toStdString().c_str());
            backend.read(provider);
            
            m_document->read(backend);
            m_document->commit();
        }
        
        bool Server::Session::authenticateUser(uint64_t user, std::string metadate) const
        {
            // @todo check token received in port metadata
            
            return true;
        }
        
        void Server::Session::bind(flip::PortBase & port)
        {
            DBG("[server] - User: "
                << std::to_string(port.user())
                << " connecting to session : "
                << hexadecimal_convert(m_identifier));
            
            if (authenticateUser(port.user(), port.metadata()))
            {
                m_document->port_factory_add(port);
                
                m_document->port_greet(port);
                
                model::Patcher& patcher = m_document->root<model::Patcher>();
                
                std::set<uint64_t> user_lit = getConnectedUsers();
                std::vector<uint64_t> users(user_lit.begin(), user_lit.end());
                
                // send a list of connected users to the user that is connecting.
                m_document->send_signal_if(patcher.signal_receive_connected_users.make(users),
                                           [&port](flip::PortBase& current_port)
                                           {
                                               return port.user() == current_port.user();
                                           });
                
                // Notify other users that this one is connected.
                m_document->send_signal_if(patcher.signal_user_connect.make(port.user()),
                                           [&port](flip::PortBase& current_port)
                                           {
                                               return port.user() != current_port.user();
                                           });
            }
            else
            {
                throw std::runtime_error("authentication failed");
            }
        }
        
        void Server::Session::unbind(flip::PortBase & port)
        {
            DBG("[server] - User " << std::to_string(port.user())
                << " disconnecting from session: " << hexadecimal_convert(m_identifier));
            
            model::Patcher& patcher = m_document->root<model::Patcher>();
            
            m_document->send_signal_if(patcher.signal_user_disconnect.make(port.user()),
                                       [](flip::PortBase& port)
                                       {
                                           return true;
                                       });
            
            m_document->port_factory_remove(port);
            
            if (m_document->ports().empty())
            {
                if(!m_backend_file.exists())
                {
                    m_backend_file.create();
                }
                
                DBG("[server] - Saving session : " << hexadecimal_convert(m_identifier)
                    << " in file : "
                    << m_backend_file.getFileName());
                
                save();
            }
        }
        
        std::set<uint64_t> Server::Session::getConnectedUsers() const
        {
            std::set<uint64_t> users;
            
            auto const& ports = m_document->ports();
            
            for(auto const& port : ports)
            {
                users.emplace(port->user());
            }
            
            return users;
        }
        
        void Server::Session::sendConnectedUsers() const
        {
            model::Patcher& patcher = m_document->root<model::Patcher>();
            
            std::set<uint64_t> user_list = getConnectedUsers();
            std::vector<uint64_t> users(user_list.begin(), user_list.end());

            m_document->reply_signal(patcher.signal_receive_connected_users.make(users));
        }
    }
}