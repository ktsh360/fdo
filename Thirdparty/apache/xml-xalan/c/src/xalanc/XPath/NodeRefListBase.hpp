/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(NODEREFLISTBASE_HEADER_GUARD_1357924680)
#define NODEREFLISTBASE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <cstddef>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;



/**
 * Local implementation of NodeRefList.  This class is for internal use only.
 */

class XALAN_XPATH_EXPORT NodeRefListBase
{
public:

	NodeRefListBase();

	virtual
	~NodeRefListBase();

#if defined(XALAN_STRICT_ANSI_HEADERS)
	typedef std::size_t		size_type;
#else
	typedef size_t			size_type;
#endif

	/**
	 * Returns the <code>index</code>th item in the collection. If
	 * <code>index</code> is greater than or equal to the number of nodes in
	 * the list, this returns <code>null</code>.
	 *
	 * @param index index into the collection
	 * @return node at the <code>index</code>th position in the
	 *         <code>NodeList</code>, or <code>null</code> if that is not a
	 *         valid index
	 */
	virtual XalanNode*
	item(size_type	index) const = 0;

	/**
	 * Determine the number of nodes in the list. The range of valid child node
	 * indices is 0 to <code>length-1</code> inclusive. 
	 *
	 * @return number of nodes
	 */
	virtual size_type
	getLength() const = 0;

	/**
	 * Retrieve the zero based index of the node in the list
	 * 
	 * @param theNode node whose index to find
	 * @return index of node
	 */
	virtual size_type
	indexOf(const XalanNode*	theNode) const = 0;

#if defined(XALAN_INLINE_INITIALIZATION)
	static const size_type	npos = ~0u;
#else
	static const size_type	npos;
#endif

protected:

	NodeRefListBase(const NodeRefListBase&)
	{
	}

	NodeRefListBase&
	operator=(const NodeRefListBase&)
	{
		return *this;
	}
};



XALAN_CPP_NAMESPACE_END



#endif	// NODEREFLISTBASE_HEADER_GUARD_1357924680