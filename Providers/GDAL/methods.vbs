'Copyright (C) 2004-2006  Autodesk, Inc.
'
'This library is free software; you can redistribute it and/or
'modify it under the terms of version 2.1 of the GNU Lesser
'General Public License as published by the Free Software Foundation.
'
'This library is distributed in the hope that it will be useful,
'but WITHOUT ANY WARRANTY; without even the implied warranty of
'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
'Lesser General Public License for more details.
'
'You should have received a copy of the GNU Lesser General Public
'License along with this library; if not, write to the Free Software
'Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Class FileSystemClass
    Private fs
    Private m_HaveError
    Private m_ErrorNumber
	
    Public Property Get HaveError
        HaveError = m_HaveError
    End Property
	
    Public Property Get ErrorNumber
        ErrorNumber = m_ErrorNumber
    End Property

    Private Sub Class_Initialize	
        Set fs = WScript.CreateObject("Scripting.FileSystemObject")
        Call ClearErr()
    End Sub

    Private Sub Class_Terminate	
        Set fs = Nothing
    End Sub

    Public Sub CopyFile(ByVal src, ByVal dest)
        Call fs.CopyFile(src, dest, True)
        CheckErr()
    End Sub
    
    Function FolderExist (ByVal folderFullPath)
        If (fs.FolderExists(folderFullPath)) Then
            FolderExist = True
        Else
            FolderExist = False
        End If
    End Function
	
    Public Sub DeleteFile(ByVal src)
        Call fs.DeleteFile(fld, True)
		
        CheckErr()
    End Sub
	
    Public Sub WriteStringToFile(ByVal sFileName, ByVal sString)
       Dim oFile
	   
       Set oFile = fs.CreateTextFile(sFileName, True)
       oFile.Write (sString)
       oFile.Close
	   
       Set oFile = Nothing
    End Sub

    Public Function ReadStringFromFile(ByVal sFileName)
    On Error Resume Next
       Dim oFile
       Dim sStr
	   
       Set oFile = fs.OpenTextFile(sFileName)
       sStr = oFile.ReadAll()
       If Err.Number = 62 Then
          sStr = ""
          Err.Clear
       Else
          Err.Raise Err.Number
       End If
	   
       oFile.Close
       ReadStringFromFile = sStr
	   
       Set oFile = Nothing
    End Function

    Function FileExist (ByVal fileFullPath)
        If (fs.FileExists(fileFullPath)) Then
            FileExist = True
        Else
            FileExist = False
        End If
    End Function
	
    Function RemoveReadOnly (ByVal fileFullPath)
        Set objFile = fs.GetFile(fileFullPath)
        If objFile.Attributes AND 1 Then
            objFile.Attributes = objFile.Attributes XOR 1
        End If
        Set objFile = Nothing
    End Function

    Private Sub CheckErr()
        If Err.number <> 0 Then 
            m_ErrorNumber = Err.number
            m_HaveError = True
			
            Err.Clear
        End If
    End Sub

    Public Sub ClearErr()
        m_ErrorNumber = Err.number
        m_HaveError = False
		
        Err.Clear
    End Sub

    Public Function ReplaceStringInFile(ByVal fileName, ByVal varName, ByVal varValue)
        Dim pos
        Dim posEnd
        Dim fileCnt
        Dim fsc
        Dim newFileCnt

        Set fsc = New FileSystemClass
        ReplaceStringInFile = False
        If fsc.FileExist(fileName) Then
    	
            fileCnt = fsc.ReadStringFromFile(fileName)
    		
            pos = InStr(1, fileCnt, varName)
    		
            If pos <> 0 Then
                posEnd = pos + Len(varName)
    		
                newFileCnt = Mid(fileCnt, 1, pos-1)
                newFileCnt = newFileCnt & varValue 
                newFileCnt = newFileCnt & Mid(fileCnt, posEnd)
                ReplaceStringInFile = True
                Call fsc.WriteStringToFile (fileName, newFileCnt)

            End If
        End If
        Set fsc = Nothing
    End Function

End Class
