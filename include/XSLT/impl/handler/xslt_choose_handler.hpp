#ifndef ARABICA_XSLT_CHOOSE_HANDLER_HPP
#define ARABICA_XSLT_CHOOSE_HANDLER_HPP

#include "../xslt_choose.hpp"

namespace Arabica
{
namespace XSLT
{

template<class stringT, class adaptorT>
class WhenHandler : public ItemContainerHandler<When<stringT, adaptorT> >
{
public:
  typedef stringT string_type;
  typedef adaptorT string_adaptor;

private:
  typedef ItemContainerHandler<When<string_type, string_adaptor> > baseT;
  typedef StylesheetConstant<string_type, string_adaptor> SC;
  typedef AttributeValidators<string_type, string_adaptor> AV;
public:
  WhenHandler(Choose<string_type, string_adaptor>* choose,
              CompilationContext<string_type, string_adaptor>& context) :
      baseT(context),
      choose_(choose)
  {
  } // WhenHandler

  virtual When<string_type, string_adaptor>* createContainer(const string_type& /* namespaceURI */,
				const string_type& /* localName */,
				const string_type& qName,
				const SAX::Attributes<string_type, string_adaptor>& atts)
  {
    static const AV rules = AV::rule(SC::test, true);
    string_type test = rules.gather(qName, atts)[SC::test];

    return new When<string_type, string_adaptor>(baseT::context().xpath_expression(test));
  } // startElement

  virtual void endElement(const string_type& /* namespaceURI */, 
			  const string_type& /* localName */, 
			  const string_type& /* qName */)
  {
    choose_->add_when(baseT::container());
    baseT::context().pop();
  } // endElement

private:
  Choose<string_type, string_adaptor>* choose_;
}; // class WhenHandler

template<class string_type, class string_adaptor>
class OtherwiseHandler : public ItemContainerHandler<Otherwise<string_type, string_adaptor> >
{
  typedef ItemContainerHandler<Otherwise<string_type, string_adaptor> > baseT;
  typedef StylesheetConstant<string_type, string_adaptor> SC;

public:
  OtherwiseHandler(Choose<string_type, string_adaptor>* choose,
                   CompilationContext<string_type, string_adaptor>& context) :
      baseT(context),
      choose_(choose)
  {
  } // OtherwiseHandler

  virtual Otherwise<string_type, string_adaptor>* createContainer(const string_type& /* namespaceURI */,
                                     const string_type& /* localName */,
                                     const string_type& /* qName */,
                                     const SAX::Attributes<string_type, string_adaptor>& atts)
  {
    if(atts.getLength())
      throw SAX::SAXException("xsl:otherwise may not have any attributes");

    return new Otherwise<string_type, string_adaptor>();
  } // createContainer

  virtual void endElement(const string_type& /* namespaceURI */,
                          const string_type& /* localName */,
                          const string_type& /* qName */)
  {
    choose_->set_otherwise(baseT::container());
    baseT::context().pop();
  } // endElement

private:
  Choose<string_type, string_adaptor>* choose_;
}; // class OtherwiseHandler

template<class string_type, class string_adaptor>
class ChooseHandler : public SAX::DefaultHandler<string_type, string_adaptor>
{
  typedef StylesheetConstant<string_type, string_adaptor> SC;
public:
  ChooseHandler(CompilationContext<string_type, string_adaptor>& context) :
      context_(context),
      choose_(0),
      seenWhere_(false),
      seenOtherwise_(false)
  {
  } // ChooseHandler

  virtual void startElement(const string_type& namespaceURI,
                            const string_type& localName,
                            const string_type& qName,
                            const SAX::Attributes<string_type, string_adaptor>& atts)
  {
    if(!choose_)
    {
      if(atts.getLength() != 0)
        throw SAX::SAXException("xsl:choose can not have attributes");

      choose_ = new Choose<string_type, string_adaptor>();
      return;
    } // if ...

    if(namespaceURI == StylesheetConstant<string_type, string_adaptor>::NamespaceURI)
    {
      if(localName == SC::when)
      {
        seenWhere_ = true;
        if(seenOtherwise_)
          throw SAX::SAXException("xsl:otherwise must be the last element in an xsl:choose");

        context_.push(0,
                      new WhenHandler<string_type, string_adaptor>(choose_, context_), 
                      namespaceURI,
                      localName,
                      qName, 
                      atts);
        return;
      } // if(localName == "when")

      if(localName == SC::otherwise)
      {
        if(seenOtherwise_)
          throw SAX::SAXException("xsl:choose may only have one xsl:otherwise element");
        seenOtherwise_ = true;
        context_.push(0,
                      new OtherwiseHandler<string_type, string_adaptor>(choose_, context_), 
                      namespaceURI,
                      localName,
                      qName, 
                      atts);
        return;
      } // if(localName == "otherwise")
    } // if ...
    throw SAX::SAXException("xsl:choose can not contain " + string_adaptor::asStdString(qName) + ".  Only xsl:when and xsl:otherwise are allowed");
  } // startElement

  virtual void endElement(const string_type& /* namespaceURI */,
                          const string_type& /* localName */,
                          const string_type& /* qName */)
  {
    if(!seenWhere_)
      throw SAX::SAXException("xsl:choose must contain at least one xsl:where element");
    context_.parentContainer().add_item(choose_);
    context_.pop();
  } // endElement

  virtual void characters(const string_type& ch)
  {
    verifyNoCharacterData<string_type, string_adaptor>(ch, SC::choose);
  } // characters

private:
  CompilationContext<string_type, string_adaptor>& context_;
  Choose<string_type, string_adaptor>* choose_;
  bool seenWhere_;
  bool seenOtherwise_;
}; // class ChooseHandler

} // namespace XSLT
} // namespace Arabica

#endif

