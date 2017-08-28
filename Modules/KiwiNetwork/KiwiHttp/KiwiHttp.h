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

#pragma once

#include <memory>
#include <chrono>
#include <functional>
#include <future>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace beast = boost::beast;

namespace kiwi { namespace network { namespace http {
    
    using Timeout = std::chrono::milliseconds;
    using Error = beast::error_code;
    
    // ================================================================================ //
    //                                      RESPONSE                                    //
    // ================================================================================ //
    
    template<class BodyType>
    class Response : public beast::http::response<BodyType>
    {
    public:
        Error error;
    };
    
    template<class BodyType>
    using Request = beast::http::request<BodyType>;
    
    //! @brief Sends an http request.
    //! @details Returns the response generated by the server. The function blocks until response
    //! is received or error occurs.
    template<class ReqType, class ResType>
    static Response<ResType>
    write(std::unique_ptr<Request<ReqType>> request,
          std::string port,
          Timeout timeout = Timeout(0));
    
    //! @brief Sends an http request asynchronously.
    //! @details Returns the response generated by the server. The function is non blocking. callback
    //! is called on another thread once response is received or error occurs.
    template <class ReqType, class ResType>
    static std::future<void>
    writeAsync(std::unique_ptr<Request<ReqType>> request,
               std::string port,
               std::function<void(Response<ResType>)> callback,
               Timeout timeout = Timeout(0));
    
    // ================================================================================ //
    //                                       QUERY                                      //
    // ================================================================================ //
    
    template<class ReqType>
    class Query
    {
    public: // methods
        
        //! @brief Constructor.
        Query(std::unique_ptr<Request<ReqType>> request,
              std::string port,
              Timeout timeout = Timeout(0));
        
        //! @brief Destructor.
        ~Query();
        
        //! @brief Write the request and get the response.
        template<class ResType>
        Response<ResType> writeRequest();
        
    private: // methods
        
        using tcp = boost::asio::ip::tcp;
        
        //! @internal
        void handleTimeout(beast::error_code const& error);
        
        //! @internal
        template<class ResType>
        void connect(Response<ResType>& response,
                     beast::error_code const& error,
                     tcp::resolver::iterator iterator);
        
        //! @internal
        template<class ResType>
        void write(Response<ResType>& response,
                   beast::error_code const& error);
        
        //! @internal
        template<class ResType>
        void read(Response<ResType>& response,
                  beast::error_code const& error);
        
        //! @internal
        void shutdown(beast::error_code const& error);
        
    private: // members
        
        std::unique_ptr<Request<ReqType>>   m_request;
        Error                               m_error;
        Timeout                             m_timeout;
        
        std::string                         m_port;
        boost::asio::io_service             m_io_service;
        tcp::socket                         m_socket;
        boost::asio::steady_timer           m_timer;
        beast::flat_buffer                  m_buffer;
        
    private: // deleted methods
        
        Query() = delete;
        Query(Query const& other) = delete;
        Query(Query && other) = delete;
        Query& operator=(Query const& other) = delete;
        Query& operator=(Query && other) = delete;
    };
    
}}} // namespace kiwi::network::http

#include "KiwiHttp.hpp"
