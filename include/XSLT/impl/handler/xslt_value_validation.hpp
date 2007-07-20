#ifndef ARABICA_XSLT_VALUE_VALIDATION_HPP
#define ARABICA_XSLT_VALUE_VALIDATION_HPP

namespace Arabica 
{
namespace XSLT
{

struct ValueRule
{
  const char* name;
  bool mandatory;
  const char* default_value;
  const char** allowed;
}; // struct ValueRule

static const char* No = "no";
static const char* Yes = "yes";
static const char* AllowedYesNo[] = { No, Yes, 0 };

void validateValues(const std::string& parentElement, const std::string& name,
                    const std::string& value, const char** allowed)
{
  for(const char* const* a = allowed; *a != 0; ++a)
    if(value == *a)
      return;

  std::ostringstream os;
  os << parentElement + ": " + name + " may be one of ";
  while(*allowed != 0)
    os << '\'' << *allowed++ << "' ";
  throw SAX::SAXException(os.str());
} // validateValues

void validateAttribute(const std::string& parentElement,
                       const std::string& name, const std::string& value, 
                       const ValueRule* rules, std::map<std::string, std::string>& results)
{
  while((rules->name != 0) && (name != rules->name))
    ++rules;

  if(rules->name == 0)
    throw SAX::SAXException(parentElement + ": Illegal attribute " + name);

  results[name] = value;

  if(rules->allowed != 0)
    validateValues(parentElement, name, value, rules->allowed);
} // validateAttribute

std::map<std::string, std::string> gatherAttributes(const std::string& parentElement,
                                                    const SAX::Attributes& atts,
                                                    const ValueRule* rules)
{
  std::map<std::string, std::string> results;

  for(const ValueRule* r = rules ; r->name != 0; ++r)
  {
    if((r->mandatory) && (atts.getValue(r->name).empty()))
      throw SAX::SAXException(parentElement + ": Attribute " + r->name + " must be specified");
    if(r->default_value)
      results[r->name] = r->default_value;
  } // 

  for(int a = 0; a < atts.getLength(); ++a)
  {
    if(atts.getLocalName(a) == "") 
      continue; // namespace decl
    validateAttribute(parentElement, atts.getLocalName(a), atts.getValue(a), rules, results);
  }

  return results;
}; // validateAttributes

} // namespace XSLT
} // namespace Arabica
#endif
