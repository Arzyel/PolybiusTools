# Polybius Modtools Programmer User Manual

In this manual some specific pipelines and systems will be explained.

## FileManager

The FileManager is the file that contains all the logic concerning data retrieval from a file and containing said data. Three classes are used. FileData, iFileDataBase and DataToken.

### iFileDataBase

This class contains the data and most methods used outside this system to interract with the data. It also contains the methods to read and write the into the files.
(insert the class schematic)

### FileData

This class is a child of `iFileDataBase` and also a template class. The template is used for two things. One to store a pointer to a target which would contain ID's representing the index of a `DataToken` inside this class. The other to store a callable passing itself with the target class. This way the system can interract with any other class as long as two other things are given to it. These are the callables to initialize the DataTokens and one to reset its data. (Maybe add an exemple ?)

### DataToken

This class is used to contain data for one specific value. Mostly a pointer into the buffer of the string containing the file and an uint_8 representing its length. It also has various variables that helps for writing back into the file.

## Add a new widget

## 