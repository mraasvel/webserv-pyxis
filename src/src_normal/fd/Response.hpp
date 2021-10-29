#pragma once
#include <string>
#include "parser/RequestParser.hpp"
#include "AFdInfo.hpp"
#include "parser/Request.hpp"
#include "MediaType.hpp"

class File;

class Response
{
	public:
		enum Status
		{
			START,
			HEADER_COMPLETE,
			COMPLETE
		};

		Response();
		~Response();

	/* Client::readEvent() */
	public:
		void	scanRequestHeader(Request const & request);
	private:
		void		setHttpVersion(int minor_version);
		void		generateAbsoluteTarget(std::string const & target_resourse);
		void		previewMethod();
		bool		isRequestError(Request const & request);
		bool			isBadRequest(Request::RequestStatus status, int request_code);
		bool			isHttpVersionError(int http_major_version);
		bool			isMethodError();
		bool				findMethod(MethodType method) const;
		bool			isExpectationError(Request const & request);
		void		continueResponse(Request const & request);

	public:
		void	executeRequest(FdTable & fd_table, Request & request);
	private:
		int			createFile(FdTable & fd_table);
		void			setFileParameter(int & access_flag, int & open_flag);
		bool			isFileReady(int access_flag);
		bool				isFileExist();
		bool				isFileAuthorized(int access_flag);
		int				openFile(int open_flag, FdTable & fd_table);
        int 		executeMethod(Request & request);
        int 			executeGet();
        int 			executePost(Request & request);
        int 			executeDelete();

	/* Client::writeEvent() */
	public:
		void	defineEncoding();
		void	checkFileError();

	public:
		void	generateResponse();
	private:
		void		responseMethod();
		void			responseGet();
		void			responsePost();
		void			responseDelete();
		void		checkFileComplete();
		void		setStringToSent();
		void			doChunked();
		void			noChunked();
		void			encodeMessageBody();
		void			setHeader();
		void				setDate();
		void				setRetryAfter();
		void				setAllow();
		void				setTransferEncodingOrContentLength();
		void					setContentLength();
		void				setContentType();
		void				setStringStatusLine();
		void				setStringHeader();


	/* utility */
	public:
		typedef RequestParser::header_field_t::const_iterator	header_iterator;
		Status				getStatus() const;
		int					getStatusCode() const;
		std::string const &	getString() const;
		void				clearString();
		void				deleteFile();
		void				updateFileEvent(FdTable & fd_table);
		bool				isFileReady() const;

	private:
		void	processError(int error_code);
		void		generateErrorPage();

	private:
		typedef	std::vector<std::string>::const_iterator		method_const_iterator;
		typedef	std::vector<std::string>::iterator				method_iterator;

		MethodType			_method;
		Status				_status;
		File*				_file;

		std::string			_absolute_target;
		AFdInfo::EventTypes	_file_event;

		RequestParser::header_field_t  _header_fields;
		std::string 		_http_version;
		int					_status_code;

		std::string			_string_to_send;
		std::string			_string_status_line;
		std::string 		_string_header;
		std::string			_message_body;

		bool				_header_sent;
		bool				_chunked;

		/* config related */
		std::vector<std::string>	_allowed_methods;
		MediaType::Map				_media_type_map;

};
