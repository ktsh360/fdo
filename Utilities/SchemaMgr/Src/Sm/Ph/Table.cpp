/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

#include "stdafx.h"
#include <Sm/Ph/Table.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Rd/PkeyReader.h>
#include <Sm/Ph/Rd/FkeyReader.h>
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/Rd/IndexReader.h>
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Error.h>

FdoSmPhTable::FdoSmPhTable(
    FdoStringP name, 
    const FdoSmPhOwner* pOwner, 
    FdoSchemaElementState elementState,
    FdoStringP pkeyName
) : 
    FdoSmPhDbObject(name, pOwner, elementState)
{
    if ( !GetExists() )
        // Use given primary key name for new tables.
        mPkeyName = pkeyName;
}

FdoSmPhTable::~FdoSmPhTable(void)
{
}

bool FdoSmPhTable::SupportsAddNotNullColumn() const
{
    return true;
}

const FdoSmPhColumnCollection* FdoSmPhTable::RefPkeyColumns() const
{
    FdoSmPhColumnsP columns = ((FdoSmPhTable*) this)->GetPkeyColumns();

    return (FdoSmPhColumnCollection*) columns;
}

FdoSmPhColumnsP FdoSmPhTable::GetPkeyColumns()
{
    LoadPkeys();

	return mPkeyColumns;
}

const FdoSmPhBatchColumnCollection* FdoSmPhTable::RefUkeyColumns() const
{
    FdoSmPhBatchColumnsP columnsColl = ((FdoSmPhTable*) this)->GetUkeyColumns();

    return (FdoSmPhBatchColumnCollection*) columnsColl;
}

FdoSmPhBatchColumnsP FdoSmPhTable::GetUkeyColumns()
{
    LoadUkeys();

	return FDO_SAFE_ADDREF( mUkeysCollection.p );
}

const FdoSmPhCheckConstraintCollection* FdoSmPhTable::RefCkeyColl() const
{
    FdoSmPhCheckConstraintsP ckeys = ((FdoSmPhTable*) this)->GetCkeyColl();

    return (FdoSmPhCheckConstraintCollection*) ckeys;
}

FdoSmPhCheckConstraintCollection* FdoSmPhTable::GetCkeyColl()
{
    LoadCkeys();

	return FDO_SAFE_ADDREF( mCkeysCollection.p );
}

const FdoSmPhIndexCollection* FdoSmPhTable::RefIndexes() const
{
    return (FdoSmPhIndexCollection*) ((FdoSmPhTable*) this)->GetIndexes();
}

FdoSmPhIndexesP FdoSmPhTable::GetIndexes()
{
    LoadIndexes();

	return mIndexes;
}

FdoSmPhColumnsP FdoSmPhTable::GetBestIdentity()
{
    int idx = 0;
    long ixSize = (FdoSmPhIndex::mMaxWeight * 2);
    long bestIxSize = (FdoSmPhIndex::mMaxWeight * 2);

    FdoSmPhColumnsP pkeyCols = GetPkeyColumns();
    FdoSmPhIndexP bestIndex;

    // Choose primary key if present
    if ( pkeyCols->GetCount() > 0 ) {
        return pkeyCols;
    }

    // No primary key, use best unique index.
    FdoSmPhIndexesP indexes = GetIndexes();

    for ( idx = 0; idx < indexes->GetCount(); idx++ ) {
        FdoSmPhIndexP index = indexes->GetItem(idx);

        // Calculate index compactness score.
        ixSize = index->GetWeight();

        // Weed out non-unique indexes or indexes whose columns are too big.
        if ( index->GetIsUnique() && (index->GetColumns()->GetCount() > 0) && (ixSize < FdoSmPhIndex::mMaxWeight) ) {
            if ( bestIndex ) {
                // Index already chosen, see if this one is better.
                FdoSmPhColumnsP idxCols = index->GetColumns();
                if (idxCols->GetCount() < bestIndex->GetColumns()->GetCount() ) {
                    // This one has few columns, take it instead.
                    bestIndex = index;
                    bestIxSize = ixSize;
                }
                else if ( idxCols->GetCount() == bestIndex->GetColumns()->GetCount() ) {
                    // same number of columns, take this index only if it has a better
                    // compactness score.
                    if ( ixSize < bestIxSize ) {
                        bestIndex = index;
                        bestIxSize = ixSize;
                    }
                }
            }
            else {
                // No index chosen yet, chose this one initially.
                bestIndex = index;
                bestIxSize = ixSize;
            }
        }
    }

    if ( bestIndex )
        return bestIndex->GetColumns();

    return (FdoSmPhColumnCollection*) NULL;
}

/* TODO
const FdoSmPhDependencyCollection* FdoSmPhTable::GetDependenciesDown() const
{
	((FdoSmPhTable*) this)->Finalize();

	return &mDependenciesDown;
}

const FdoSmPhDependencyCollection* FdoSmPhTable::GetDependenciesUp() const
{
	((FdoSmPhTable*) this)->Finalize();

	return &mDependenciesUp;
}
*/

const FdoSmPhFkeyCollection* FdoSmPhTable::RefFkeysUp() const
{
    return (FdoSmPhFkeyCollection*) ((FdoSmPhTable*) this)->GetFkeysUp();
}

FdoSmPhFkeysP FdoSmPhTable::GetFkeysUp()
{
    LoadFkeys();

	return mFkeysUp;
}

FdoStringP FdoSmPhTable::GetPkeyName() const
{
	((FdoSmPhTable*) this)->GetPkeyColumns();

	return mPkeyName;
}

const FdoLockType* FdoSmPhTable::GetLockTypes(FdoInt32& size) const
{
    FdoSmPhOwner* pOwner = static_cast<FdoSmPhOwner*>((FdoSmPhSchemaElement*) GetParent());

    // Get the supported lock types for this table's locking mode.
    return pOwner->GetLockTypes( GetLockingMode(), size );
}

void FdoSmPhTable::SetPkeyName( FdoStringP pkeyName )
{
    if ( GetElementState() != FdoSchemaElementState_Added )
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_20), 
				(FdoString*) GetQName()
            )
        );

    mPkeyName = pkeyName;
}

void FdoSmPhTable::SetLtMode( FdoLtLockModeType mode )
{
    if ( (mode != GetLtMode()) && (GetElementState() != FdoSchemaElementState_Added) ) {
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_23),
				(FdoString*) GetDbQName() 
			)
		);
    }

    FdoSmPhDbObject::SetLtMode( mode );
}

void FdoSmPhTable::SetLockingMode( FdoLtLockModeType mode )
{
    if ( (mode != GetLockingMode()) && (GetElementState() != FdoSchemaElementState_Added) ) {
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_24),
				(FdoString*) GetDbQName() 
			)
		);
    }

    FdoSmPhDbObject::SetLockingMode( mode );
}

void FdoSmPhTable::AddPkeyCol(FdoStringP columnName )
{
    LoadPkeys();

	FdoSmPhColumnP column = GetColumns()->FindItem( columnName );

	if ( column ) 
		mPkeyColumns->Add( column );
	else
		throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_213),				
   				(FdoString*) columnName, 
				GetName()
			)
		);
}

void FdoSmPhTable::AddUkeyCol(int uCollNum, FdoStringP columnName )
{
    LoadUkeys();

	FdoSmPhColumnP column = GetColumns()->FindItem( columnName );

	FdoSmPhColumnsP	ukeyColumns = mUkeysCollection->GetItem( uCollNum );

	if ( column ) 
		ukeyColumns->Add( column );
	else
		throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_411),				
   				(FdoString*) columnName, 
				GetName()
			)
		);
}

void FdoSmPhTable::AddCkeyCol(FdoSmPhCheckConstraintP clause )
{
    LoadCkeys();

	mCkeysCollection->Add( clause );
}

FdoSmPhIndexP FdoSmPhTable::CreateIndex(
    FdoStringP name, 
    bool isUnique
)
{
    FdoSmPhIndexP index = NewIndex( name, isUnique );

    FdoSmPhIndexesP indexes = GetIndexes();
    indexes->Add( index );

    return index;
}

FdoSmPhFkeyP FdoSmPhTable::CreateFkey(
    FdoStringP name, 
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner
)
{
    FdoStringP lTableOwner = pkeyTableOwner;

    if ( lTableOwner == L"" ) 
        lTableOwner = GetParent()->GetName();

    FdoSmPhFkeyP fkey = NewFkey( name, pkeyTableName, lTableOwner );

    FdoSmPhFkeysP fkeys = GetFkeysUp();
    fkeys->Add( fkey );

    return fkey;
}

void FdoSmPhTable::DiscardIndex( FdoSmPhIndex* index )
{
    FdoSmPhIndexesP indexes = GetIndexes();
    indexes->Remove( index );
}

void FdoSmPhTable::CommitFkeys( bool isBeforeParent )
{
    FdoInt32        i;
    FdoSmPhFkeysP   fkeys = GetFkeysUp();
    FdoStringsP     fkeyClauses = FdoStringCollection::Create();

    for ( i = (fkeys->GetCount() - 1); i >= 0; i-- ) {
        FdoSmPhFkeyP(fkeys->GetItem(i))->Commit(true, isBeforeParent);
    }
}

void FdoSmPhTable::SetElementState(FdoSchemaElementState elementState)
{
	// Error if try to drop a table that has rows.
    if ( elementState == FdoSchemaElementState_Deleted ) {
        if ( GetHasData() ) { 
            FdoSmPhSchemaElement::SetElementState( elementState );
    		AddDeleteNotEmptyError();
        }
        else {
            ForceDelete();
        }
	}
    else {
        FdoSmPhSchemaElement::SetElementState( elementState );
    }
}

void FdoSmPhTable::ForceDelete()
{
    FdoInt32 i;

  	FdoSmPhSchemaElement::SetElementState( FdoSchemaElementState_Deleted );

    // Must explicitly delete any foreign keys when table is deleted.
    FdoSmPhFkeysP   fkeys = GetFkeysUp();
    for ( i = 0; i < fkeys->GetCount(); i++ ) {
        fkeys->GetItem(i)->SetElementState(FdoSchemaElementState_Deleted);
    }
}

FdoSchemaExceptionP FdoSmPhTable::Errors2Exception(FdoSchemaException* pFirstException ) const
{
    FdoInt32 i;

	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmPhDbObject::Errors2Exception(pFirstException);

    if ( RefColumns()->GetCount() == 0 ) {
        pException = FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_15), 
				(FdoString*) GetQName()
            ),
            pException
        );
    }

    if ( mIndexes ) {
        // Add errors for the table's columns.
	    for ( i = 0; i < mIndexes->GetCount(); i++ )
		    pException = mIndexes->RefItem(i)->Errors2Exception(pException);
    }

    if ( mFkeysUp ) {

	    // Add errors for the table's columns.
	    for ( i = 0; i < mFkeysUp->GetCount(); i++ )
		    pException = mFkeysUp->RefItem(i)->Errors2Exception(pException);
    }

    if ( (GetElementState() == FdoSchemaElementState_Unchanged) ||
         (GetElementState() == FdoSchemaElementState_Modified)
    ) {
        // Table exists 

        if ( ((FdoSmPhTable*)this)->GetHasData() || !SupportsAddNotNullColumn() ) {
            // Not null columns cannot be added if the table has rows, or the current
            // RDBMS simply does not support adding not nul columns.
            // Log an error for each attempt to add such a column.
            const FdoSmPhColumnCollection* columns = RefColumns();
            
            for ( i = 0; i < columns->GetCount(); i++ ) {
                const FdoSmPhColumn* column = columns->RefItem(i);
                if ( (column->GetElementState() == FdoSchemaElementState_Added) &&
                     !column->GetNullable()
                ) {
                    // Found a not null column being added.
                    // Log an error. The message depends on whether the reason
                    // is that adding these columns is not supported or the 
                    // table has rows.
                    if ( SupportsAddNotNullColumn() ) {
                        pException = FdoSchemaException::Create(
                            FdoSmError::NLSGetMessage(
                                FDO_NLSID(FDOSM_218), 
				                (FdoString*) GetQName(),
                                column->GetName()
                            ),
                            pException
                        );
                    }
                    else {
                        pException = FdoSchemaException::Create(
                            FdoSmError::NLSGetMessage(
                                FDO_NLSID(FDOSM_16),
                                column->GetName(),
				                (FdoString*) GetQName()
                            ),
                            pException
                        );
                    }
                }
            }
        }
    }

	return pException;
}

void FdoSmPhTable::CommitChildren( bool isBeforeParent )
{
    int                     i;

    // Columns to add must be committed before indexes to add
    if ( !isBeforeParent )
        CommitColumns( isBeforeParent );

    if ( mIndexes ) {
        // Indexes to drop must be dropped before table is modified.
        // Process in reverse order because indexes are removed from 
        // this cache as they are deleted.
        for ( i = (mIndexes->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhIndexP index = mIndexes->GetItem(i);
            index->Commit( true, isBeforeParent );
        }
    }

    // Columns to drop must be committed after indexes to drop
    if ( isBeforeParent )
        CommitColumns( isBeforeParent );

}

void FdoSmPhTable::CommitColumns( bool isBeforeParent )
{
    FdoSchemaElementState elementState = GetElementState();
    int i;

    FdoSmPhColumnsP columns = GetColumns();

    for ( i = 0; i < columns->GetCount(); i++ ) {
        FdoSmPhColumnP column = columns->GetItem(i);

        FdoSchemaElementState colState = column->GetElementState();
        bool actionComplete = false;

        if ( isBeforeParent &&
            (elementState != FdoSchemaElementState_Added) &&
            (colState == FdoSchemaElementState_Added)
        )
            // new columns must be added after mods to existing table are committed.
            // modified and deleted columns are processed before table is committed.
            continue;

        // Call provider-specific implementor depending on the modification action.
        switch ( colState ) {
        case FdoSchemaElementState_Added:
            actionComplete = AddColumn(column);
	        break;

	    case FdoSchemaElementState_Deleted:
            actionComplete = DeleteColumn(column);
	        break;

	    case FdoSchemaElementState_Modified:	
            actionComplete = ModifyColumn(column);
	        break;
	    }

        if ( actionComplete ) {
            if ( colState == FdoSchemaElementState_Deleted ) {
                column->SetElementState( FdoSchemaElementState_Detached );
                // Remove deleted columns from the cache.
                columns->Remove( (FdoSmPhColumn*) column );
            }
            else {
                column->SetElementState( FdoSchemaElementState_Unchanged );
            }
        }
    }
}

void FdoSmPhTable::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<table name=\"%ls\" description=\"%ls\" pkeyName=\"%ls\" %ls>\n",
			GetName(), GetDescription(), (FdoString*) GetPkeyName(),
            (FdoString*) XMLSerializeProviderAtts()
	);

	if ( ref == 0 ) {
		for ( int i = 0; i < RefColumns()->GetCount(); i++ ) 
			RefColumns()->RefItem(i)->XMLSerialize(xmlFp, ref);
	}

	fprintf( xmlFp, "</table>\n" );

}

FdoStringP FdoSmPhTable::GetAddSql()
{
    FdoStringP pkeySql = GetAddPkeySql();
	FdoStringP ukeysSql = GetAddUkeySql();
	FdoStringP ckeysSql = GetAddCkeySql();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"%ls ( %ls%ls %ls%ls %ls%ls %ls )",
        (FdoString*) GetAddHdgSql(),
        (FdoString*) GetAddColsSql()->ToString(),
        (ckeysSql == L"" ) ? L"" : L", ",
        (FdoString*) ckeysSql,
        (ukeysSql == L"" ) ? L"" : L", ",
        (FdoString*) ukeysSql,
        (pkeySql == L"" ) ? L"" : L", ",
        (FdoString*) pkeySql
    );

    return sqlStmt;
}

FdoStringP FdoSmPhTable::GetAddHdgSql()
{
    return FdoStringP::Format( 
        L"create table %ls", 
        (FdoString*) GetQName() 
    );
}

FdoStringP FdoSmPhTable::GetDeleteSql()
{
    return FdoStringP::Format( 
        L"drop table %ls", 
        (FdoString*) GetDbQName() 
    );
}

FdoStringsP FdoSmPhTable::GetAddColsSql()
{
    FdoInt32        i;
    FdoSmPhColumnsP columns = GetColumns();
    FdoStringsP     colClauses = FdoStringCollection::Create();

    for ( i = 0; i < columns->GetCount(); i++ ) {
        colClauses->Add( FdoSmPhColumnP(columns->GetItem(i))->GetAddSql() );
    }

    return colClauses;
}

FdoStringP FdoSmPhTable::GetAddColSql()
{
    return FdoStringP::Format( 
        L"alter table %ls add", 
        (FdoString*) GetDDLQName() 
    );
}

FdoStringP FdoSmPhTable::GetDeleteColSql()
{
    return FdoStringP::Format( 
        L"alter table %ls drop column ", 
        (FdoString*) GetDDLQName() 
    );
}

FdoStringP FdoSmPhTable::GetAddPkeySql()
{
    FdoInt32            i;
    FdoSmPhColumnsP     pkeyColumns = GetPkeyColumns();
    FdoStringsP         pkColNames = FdoStringCollection::Create();
    FdoStringP          pkeySql;
	bool				autoincrColAdded = false;

    if ( pkeyColumns->GetCount() > 0 ) {
	    for ( i = 0; i < pkeyColumns->GetCount(); i++ )
		{
			FdoSmPhColumnP	pkeyColumn = pkeyColumns->GetItem(i);
	        pkColNames->Add( pkeyColumn->GetName() );
			if ( !autoincrColAdded && pkeyColumn->GetAutoincrement() )
				autoincrColAdded = true;
		}

		// Add the autoincremented column as a primary key
		if ( !autoincrColAdded )
		{
			FdoSmPhColumnsP columns = GetColumns();
			for ( i = 0; i < columns->GetCount(); i++ )
			{
				FdoSmPhColumnP	column = columns->GetItem(i);
				if ( column->GetAutoincrement() )
					pkColNames->Add( column->GetName() );
			}		
		}

        pkeySql = FdoStringP::Format( 
            L"constraint %ls primary key ( %ls )",
            (FdoString*) this->GenPkeyName(),
            (FdoString*) pkColNames->ToString()
        );
    }

    return pkeySql;
}

FdoStringP FdoSmPhTable::GetAddUkeySql()
{
    FdoSmPhBatchColumnsP     ukeyColumnsColl = GetUkeyColumns();
	int						 count = ukeyColumnsColl->GetCount();
    FdoStringP				 ukeySql;
	FdoStringP				 ukeyCollSql;

    if ( count > 0 ) {

		for ( int i = 0; i < count; i++ )	{

			FdoSmPhColumnsP     ukeyColumns = ukeyColumnsColl->GetItem(i);
			FdoStringsP			ukColNames = FdoStringCollection::Create();

			if ( ukeyColumns->GetCount() > 0 ) {
				for ( int j = 0; j < ukeyColumns->GetCount(); j++ )	{
					FdoSmPhColumnP	ukeyColumn = ukeyColumns->GetItem(j);
					ukColNames->Add( ukeyColumn->GetName() );
				}

				ukeySql = FdoStringP::Format( 
					L"UNIQUE (%ls)",
					(FdoString*) ukColNames->ToString()
				);

				ukeyCollSql += ukeySql;	
				if ( i != count - 1 )
					ukeyCollSql += L", ";
			}	
		}
    }

    return ukeyCollSql;
}

FdoStringP FdoSmPhTable::GetAddCkeySql()
{
    FdoSmPhCheckConstraintsP	ckeyColl = GetCkeyColl();
	int							count = ckeyColl->GetCount();
	FdoStringP					ckeyCollSql;
 
    if ( count > 0 ) {

		for ( int i = 0; i < count; i++ )	{

			FdoSmPhCheckConstraintP	elem = ckeyColl->GetItem(i);

			FdoStringP ckeySql = FdoStringP::Format( 
				L"CHECK (%ls)",
				(FdoString*) elem->GetClause()
			);

			ckeyCollSql += ckeySql;	
			if ( i != count - 1 )
				ckeyCollSql += L", ";		
		}
    }

    return ckeyCollSql;
}

FdoStringP FdoSmPhTable::GenPkeyName()
{
    if ( mPkeyName == L"" ) {
        FdoSmPhOwner* pOwner = dynamic_cast<FdoSmPhOwner*>((FdoSmPhSchemaElement*) GetParent());
        mPkeyName = pOwner->UniqueDbObjectName( FdoStringP(L"PK_") + GetName() );
    }

    return mPkeyName;
}

void FdoSmPhTable::LoadPkeys(void)
{
    // Do nothing if primary key already loaded
	if ( !mPkeyColumns ) {
        mPkeyColumns = new FdoSmPhColumnCollection();

        // Skip load if new table.
        if ( GetElementState() != FdoSchemaElementState_Added ) {
            FdoPtr<FdoSmPhRdPkeyReader> pkeyRdr = CreatePkeyReader();

            // read each primary key column.
            while (pkeyRdr->ReadNext() ) {
    	        mPkeyName = pkeyRdr->GetString(L"", L"constraint_name");
                FdoStringP columnName = pkeyRdr->GetString(L"",L"column_name");

                FdoSmPhColumnP pkeyColumn = GetColumns()->GetItem( columnName );

    	        if ( pkeyColumn == NULL ) {
                    // Primary Key column must be in this table.
			        if ( GetElementState() != FdoSchemaElementState_Deleted )
				        AddPkeyColumnError( columnName );
		        }
		        else {
			        mPkeyColumns->Add(pkeyColumn);
		        }
            }
        }
    }
}

void FdoSmPhTable::LoadUkeys()
{
    // Do nothing if unique constraints already loaded
	if ( !mUkeysCollection ) {
        mUkeysCollection = new FdoSmPhBatchColumnCollection();

		// Quick exit if nothing to do.
		if ( wcscmp(this->GetName(), L"F_CLASSDEFINITION" ) == 0 )
			return;

        // Skip load if new table.
        if ( GetElementState() != FdoSchemaElementState_Added ) {

			FdoSmPhOwner* pOwner = static_cast<FdoSmPhOwner*>((FdoSmPhSchemaElement*) GetParent());
			FdoPtr<FdoSmPhRdConstraintReader> ukeyRdr = pOwner->CreateConstraintReader(GetName(), L"U");

			FdoStringP		 ckeyNameCurr;
			FdoSmPhColumnsP  ukeysCurr;

            // read each unique key.
            while (ukeyRdr->ReadNext() ) {

    	        FdoStringP ckeyName			= ukeyRdr->GetString(L"", L"constraint_name");
                FdoStringP columnName		= ukeyRdr->GetString(L"", L"column_name");

                FdoSmPhColumnP ukeyColumn = GetColumns()->GetItem( columnName );

                // Unique Key column must be in this table.
    	        if ( ukeyColumn == NULL ) {
			        if ( GetElementState() != FdoSchemaElementState_Deleted )
				        AddUkeyColumnError( columnName );
		        }

				// The subcollection is identified by the common ckeyName.
				// The columns will be grouped this way.
				if ( ckeyName != ckeyNameCurr ) {
					if ( ukeysCurr )
						mUkeysCollection->Add( ukeysCurr ); // save the last group

					// Start a new subcollection
					ukeysCurr = new FdoSmPhColumnCollection();
				}		
				ukeysCurr->Add( ukeyColumn );

				ckeyNameCurr = ckeyName;		
            }

			// Add the last group
			if ( ukeysCurr )
				mUkeysCollection->Add( ukeysCurr );
        }
    }
}

void FdoSmPhTable::LoadCkeys()
{
    // Do nothing if check constraints already loaded
	if ( !mCkeysCollection ) {
		mCkeysCollection = new FdoSmPhCheckConstraintCollection();

		// Quick exit if nothing to do.
		if ( wcscmp(this->GetName(), L"F_CLASSDEFINITION" ) == 0 )
			return;

        // Skip load if new table.
        if ( GetElementState() != FdoSchemaElementState_Added ) {
			FdoSmPhOwner* pOwner = static_cast<FdoSmPhOwner*>((FdoSmPhSchemaElement*) GetParent());
			FdoPtr<FdoSmPhRdConstraintReader> ckeyRdr = pOwner->CreateConstraintReader(GetName(), L"C");

			// MySql provider does not support CHECK() and the reader is NULL

            // read each primary key column.
            while (ckeyRdr && ckeyRdr->ReadNext() ) {

    	        FdoStringP ckeyName			= ckeyRdr->GetString(L"", L"constraint_name");
                FdoStringP columnName		= ckeyRdr->GetString(L"", L"column_name");
				FdoStringP clause			= ckeyRdr->GetString(L"", L"check_clause");

				if ( clause == L"" || clause.Contains(L"NOT NULL"))
					continue;

                FdoSmPhColumnP ckeyColumn = GetColumns()->GetItem( columnName );

                // Cheked column must be in this table.
    	        if ( ckeyColumn == NULL ) {
			        if ( GetElementState() != FdoSchemaElementState_Deleted )
				        AddCkeyColumnError( columnName );
		        }

				FdoSmPhCheckConstraint  *pConstr = new FdoSmPhCheckConstraint( ckeyName, columnName, clause );
			    mCkeysCollection->Add( pConstr );
            }
        }
	}
}


void FdoSmPhTable::LoadFkeys(void)
{
    // Do nothing if already loaded
	if ( !mFkeysUp ) {
        mFkeysUp = new FdoSmPhFkeyCollection();

        // Skip load for new tables
        if ( GetElementState() != FdoSchemaElementState_Added ) {
            FdoStringP                  nextFkey;
            FdoSmPhFkeyP                fkey;
            FdoPtr<FdoSmPhRdFkeyReader> fkeyRdr = CreateFkeyReader();

            // Read each foreign key and column
            while ( fkeyRdr->ReadNext() ) {
                nextFkey = fkeyRdr->GetString(L"",L"constraint_name");

                if ( !fkey || (nextFkey != fkey->GetName()) ) {
                    // hit the next foreign key. Create an object for it
                    fkey = NewFkey(
                        nextFkey, 
                        fkeyRdr->GetString(L"", "r_table_name"),
                        fkeyRdr->GetString(L"", "r_owner_name"),
                        FdoSchemaElementState_Unchanged
                    );

                    mFkeysUp->Add(fkey);
                }

                // Add the column to the foreign key
                FdoStringP columnName = fkeyRdr->GetString(L"",L"column_name");
                FdoSmPhColumnP column = GetColumns()->GetItem(columnName);

                if ( column ) {
                    fkey->AddFkeyColumn( 
                        column,
                        fkeyRdr->GetString(L"", "r_column_name")
                    );
                }
                else {
                    // Foreign Key column must be in this table.
			        if ( GetElementState() != FdoSchemaElementState_Deleted )
				        AddFkeyColumnError( columnName );
                }
            }
        }
	}
}

void FdoSmPhTable::LoadIndexes(void)
{
    // Do nothing if already loaded
	if ( !mIndexes ) {
        mIndexes = new FdoSmPhIndexCollection();

        FdoStringP              nextIndex;
        FdoSmPhIndexP         index;

        // Skip load if table is new
        if ( GetElementState() != FdoSchemaElementState_Added ) {
            FdoPtr<FdoSmPhRdIndexReader> indexRdr = CreateIndexReader();

            // Read each index and column
            while ( indexRdr->ReadNext() ) {
                nextIndex = indexRdr->GetString(L"",L"index_name");

                if ( !index || (nextIndex != index->GetName()) ) {
                    // hit the next index. Create an object for it
                    index = CreateIndex( indexRdr ); 
                        
                    if ( index ) 
                        mIndexes->Add(index);
                }

                FdoStringP columnName = indexRdr->GetString(L"",L"column_name");
                FdoSmPhColumnP column = GetColumns()->GetItem(columnName);

                if ( column ) {
                    // Add the column to the current index.
                    index->AddColumn( column );
                }
                else {
                    // Index column must be in this table.
			        if ( GetElementState() != FdoSchemaElementState_Deleted )
				        AddIndexColumnError( columnName );
                }
            }
        }
    }
}

FdoSmPhIndexP FdoSmPhTable::CreateIndex(
    FdoPtr<FdoSmPhRdIndexReader> rdr
)
{
    FdoSmPhIndexP index;

    switch ( rdr->GetIndexType() ) {
    case FdoSmPhIndexType_Scalar:
        index = NewIndex(
            rdr->GetString(L"",L"index_name"), 
            (rdr->GetString(L"",L"uniqueness") == L"UNIQUE") ? true : false,
            FdoSchemaElementState_Unchanged
        );
        break;

    case FdoSmPhIndexType_Spatial:
        index = NewSpatialIndex(
            rdr->GetString(L"",L"index_name"), 
            (rdr->GetString(L"",L"uniqueness") == L"UNIQUE") ? true : false,
            FdoSchemaElementState_Unchanged
        );
        break;
    }

    return index;
}

void FdoSmPhTable::AddPkeyColumnError(FdoStringP columnName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_217), 
				(FdoString*) columnName, 
				(FdoString*) GetQName()
            )
        )
	);
}

void FdoSmPhTable::AddUkeyColumnError(FdoStringP columnName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_410), 
				(FdoString*) columnName, 
				(FdoString*) GetQName()
            )
        )
	);
}

void FdoSmPhTable::AddCkeyColumnError(FdoStringP columnName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_411), 
				(FdoString*) columnName, 
				(FdoString*) GetQName()
            )
        )
	);
}

void FdoSmPhTable::AddFkeyColumnError(FdoStringP columnName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_2), 
				(FdoString*) columnName, 
				(FdoString*) GetQName()
            )
        )
	);
}
void FdoSmPhTable::AddIndexColumnError(FdoStringP columnName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_3), 
				(FdoString*) columnName, 
				(FdoString*) GetQName()
            )
        )
	);
}

void FdoSmPhTable::AddDeleteNotEmptyError(void)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_216), 
				(FdoString*) GetDbQName()	
			)
		)
	);
}

