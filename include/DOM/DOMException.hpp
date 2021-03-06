#ifndef JEZUK_DOM_EXCEPTION_H
#define JEZUK_DOM_EXCEPTION_H

////////////////////////////
// C++ DOM definition
////////////////////////////

#include <stdexcept>
#include <typeinfo>

namespace Arabica
{
namespace DOM
{

class DOMBadCast : public std::bad_cast
{
public:
  DOMBadCast(const char* expectedType) :
      message_(std::string("Cannot cast to ") + expectedType)
  {
  } // DOMBadCast

  DOMBadCast(const DOMBadCast& rhs) :
      message_(rhs.message_)
  {
  } // DOMBadCase

  virtual ~DOMBadCast() throw()
  {
  } // DOMBadCast

  virtual const char* what() const throw()
  {
    return message_.c_str();
  } // what

private:
  const std::string message_;

  DOMBadCast& operator=(const DOMBadCast&);
  bool operator==(const DOMBadCast&) const;
}; // DOMBadCast
      
class DOMException : public std::runtime_error
{
public:
  enum CODE 
  {
    INDEX_SIZE_ERR = 1,
    DOMSTRING_SIZE_ERR = 2,
    HIERARCHY_REQUEST_ERR = 3,
    WRONG_DOCUMENT_ERR = 4,
    INVALID_CHARACTER_ERR = 5,
    NO_DATA_ALLOWED_ERR = 6,
    NO_MODIFICATION_ALLOWED_ERR = 7,
    NOT_FOUND_ERR = 8,
    NOT_SUPPORTED_ERR = 9 ,
    INUSE_ATTRIBUTE_ERR = 10,
    INVALID_STATE_ERR,
    SYNTAX_ERR,
    INVALID_MODIFICATION_ERR,
    NAMESPACE_ERR,
    INVALID_ACCESS_ERR
  }; // enum CODE

  DOMException(CODE code) : 
    std::runtime_error("DOMException"), 
    code_(code) 
  { 
  } // DOMException

  DOMException(const DOMException& rhs) :
    std::runtime_error(rhs),
    code_(rhs.code_)
  {
  } // DOMException

  virtual ~DOMException() throw()
  {
  } // DOMBadCast

  CODE code() const { return code_; }

  virtual const char* what() const throw()
  {
    switch(code_)
    {
      case INDEX_SIZE_ERR:
        return "Index size error";
      case DOMSTRING_SIZE_ERR:
        return "DOMString size error";
      case HIERARCHY_REQUEST_ERR:
        return "Hierarchy request error";
      case WRONG_DOCUMENT_ERR:
        return "Wrong Document error";
      case INVALID_CHARACTER_ERR:
        return "Invalid Character error";
      case NO_DATA_ALLOWED_ERR:
        return "No data allowed error";
      case NO_MODIFICATION_ALLOWED_ERR:
        return "No modification allowed error";
      case NOT_FOUND_ERR:
        return "Not found error";
      case NOT_SUPPORTED_ERR:
        return "Not supported error";
      case INUSE_ATTRIBUTE_ERR:
        return "Attribute inuse error";
      case INVALID_STATE_ERR:
        return "Invalid state";
      case SYNTAX_ERR:
        return "Syntax error";
      case INVALID_MODIFICATION_ERR:
        return "Invalid modification error";
      case NAMESPACE_ERR:
        return "Namespace error";
      case INVALID_ACCESS_ERR:
        return "Invalid access error";
    } // switch(code_)

    return "DOM error";
  } // what

private:
  DOMBadCast& operator=(const DOMBadCast&);
  bool operator==(const DOMBadCast&) const;

  CODE code_;
}; // class DOMException

} // namespace DOM
} // namespace Arabica

#endif

