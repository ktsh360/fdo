#ifndef FDOSMPHGRDSYNONYM_H
#define FDOSMPHGRDSYNONYM_H        1
/*
 * (C) Copyright 2011 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 */
#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/Synonym.h>

// represents a Synonym
class FdoSmPhGrdSynonym   : public FdoSmPhSynonym
{
public:
    FdoSmPhGrdSynonym(
        FdoStringP name,                            // synonym name
        FdoSmPhDbObjectP rootObject,                // the object referenced by this synonym.
                                                    // This parameter is mandatory for new synonyms
                                                    // It must be NULL for other synonyms. In this case
                                                    // the referenced object is read from the RDBMS.
        const FdoSmPhOwner* pOwner,                 // datastore containing the synonym
		FdoSchemaElementState elementState = FdoSchemaElementState_Added,
                                                    // Indicates whether synonym already exists.
        FdoSmPhRdDbObjectReader* reader = NULL      // Provides info for existing synonym
    );
    ~FdoSmPhGrdSynonym();

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhGrdSynonym() {}

private:
    // Post all synonym modifications to the RDBMS
    // Providers can override if custom SQL required. 
	virtual bool Add();
	virtual bool Delete();
	virtual bool Modify();
};

typedef FdoPtr<FdoSmPhGrdSynonym> FdoSmPhGrdSynonymP;

#endif
