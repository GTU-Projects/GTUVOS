#include <iostream>
#include "gtuvos.h"
#include <vector>
#include <QApplication>
#include <QVector>

// begin of the constant variables about copyFile function
const static int  SUCCESS_STATUS = 0;
const static int  SUCCESS_COPY_FILE = 0;
const static int  IF_THE_FILE_ALREADY_EXISTS_DO_NOTHING = 1;
const static int  IF_THE_FILE_ALREADY_EXISTS_CHANGE_FILE_NAME = 2;
const static int  IF_THE_FILE_ALREADY_EXISTS_OVERWRITE = 3;
const static int  FAIL_COPY_FILE = -1;
const static int  FILE_PATH_NOT_EXIST_OR_NOT_A_FILE = -2;
const static int  TARGET_PATH_NOT_EXIST_OR_NOT_A_DIRECTORY = -3;
const static int  CREATED_FILE_DELETED_DURING_COPY_PROCESS = -4;
// end of the constant variables about copyFile function


using namespace std;

GTUVOS *GTUVOS::instance= NULL;

GTUVOS::GTUVOS(){
    name = "GTU Virtual OS";
    version = 0.1;

    prepareSystem();

    cout<<"GTUVOS constructed!"<<endl;
}

void GTUVOS::prepareSystem(){
    cout<< "GTUVOS prepareSystem started!!"<<endl;
}

/* Function takes a QString type string and splits it according to spaces in it, then returns that
 * in a vector form. If the str's size is 0, the "Insufficient Tokens" will be pushed to de vector and returns vector instantly.
 * If the size is not 0, for each element in stringList, which was already split by whitespaces, will be transformed into a
 * standart string from QString type and then it is pushed to the return vector which named as parses.
 *
 * @param str the QString type string
 * @return A vector of strings. Each element is parsed and will not include any whitespaces. *
 */
vector<string> GTUVOS::parseStr(QString str){
    cout<<endl<<"parseStr is started, param: "<<str.toStdString()<<endl;

    QStringList stringlist =str.split(" ");
    vector<string> parses;

    if(str.size() == 0){
        parses.push_back("InsufficientNumberOfTokens");
        return parses;
    }

    foreach (QString iter, stringlist) {
       parses.push_back(iter.toStdString());
    }

    return parses;
}

/* This function executes the command if possible and returns true. To decide this it calls the parseStr function and
 * looks at the return value's first element.
 * Else if not possible function will return false.
 *
 * @param str the QString type string which contains full command and parameters for commands if there can be.
 * @return If it's an available command returns true, else returns false.
 *
 */
bool GTUVOS::executeCmd(QString str){
    vector<string> parses = parseStr(str);// Call of parseStr function.

    foreach (string tempStr, parses) {
        cout<<tempStr<<endl;
    }

    string command = parses.front();// First element of vector parses. Used for deciding the command.

    if(command == "cp"){
        cout<<"Command copyfile will be executed."<<endl<<endl;
    }else if (command == "mail"){
        cout<<"Mailbox window will be opened."<<endl<<endl;
    }else if (command == "help" || command == "Help" ){
        cout<<"The available commands are:"<<endl
            <<"help"<<endl
            <<"cp <file_1> <file_2>"<<endl
            <<"mail"<<endl<<endl;
    }else if (command == "exit" || command == "Exit" || command == "quit" || command == "Quit" ){
        cout<<" ~~~~ System shutdown command entered! ~~~~ "<<endl<<endl;
    }else{
        cout<<"There is no such command."<<endl<<endl;
        return false;
    }


    return true;
}

double GTUVOS::getVersion() const{
    return version;
}

string GTUVOS::getName() const{
    return name;
}

/*
 * Copy the file to the target path
 *
 * SUCCESSFUL CASES:
 *
 * If target path have not a file which have a same name like given file name then:
 *      -if the file copy process happens, then return SUCCESS_COPY_FILE.
 *
 * If target path have a file which have a same name like given file name then:
 *      -if ifTheFileAlreadyExists IF_THE_FILE_ALREADY_EXISTS_DO_NOTHING, then do nothing and return IF_THE_FILE_ALREADY_EXISTS_DO_NOTHING
 *      -if ifTheFileAlreadyExists IF_THE_FILE_ALREADY_EXISTS_CHANGE_FILE_NAME, then change the file name which will created and return IF_THE_FILE_ALREADY_EXISTS_CHANGE_FILE_NAME
 *      -if ifTheFileAlreadyExists IF_THE_FILE_ALREADY_EXISTS_OVERWRITE, then overwrite the existing file and return IF_THE_FILE_ALREADY_EXISTS_OVERWRITE
 *
 *
 * UNSUCCESSFUL CASES:
 *
 * If the file copy process fails, then return FAIL_COPY_FILE.
 * If filePath (include file name) is not exist or is not a file name then return FILE_PATH_NOT_EXIST_OR_NOT_A_FILE.
 * If targetPath is not exist or is not a directory name then return TARGET_PATH_NOT_EXIST_OR_NOT_A_DIRECTORY.
 * if the created file was deleted during the process then return CREATED_FILE_DELETED_DURING_COPY_PROCESS.
 *
 * @param filePath which is the path of the file to be copied (include file name)
 * @param targetPath which  the path of the target directory
 * @param ifTheFileAlreadyExists can be IF_THE_FILE_ALREADY_EXISTS_DO_NOTHING or IF_THE_FILE_ALREADY_EXISTS_CHANGE_FILE_NAME or IF_THE_FILE_ALREADY_EXISTS_OVERWRITE.
 * @param theNameOfTheCopyFileCreated is file name which will created. If the function fails then it does not matter.
 * @return It depends on the situation.
 */
int GTUVOS::copyFile(string filePath, string targetPath, int ifTheFileAlreadyExists, string* theNameOfTheCopyFileCreated){

    // Check is filePath a file name?
    if(!isItAFile(filePath) || isItADirectory(filePath))
        return FILE_PATH_NOT_EXIST_OR_NOT_A_FILE;

    // Check is targetPath a directory name?
    if(!isItADirectory(targetPath))
        return TARGET_PATH_NOT_EXIST_OR_NOT_A_DIRECTORY;

    // Open the source file
    int source = open(filePath.c_str(), O_RDONLY, 0);

    /* If filePath didn't open */
    if(source < SUCCESS_STATUS)
        return FILE_PATH_NOT_EXIST_OR_NOT_A_FILE;

    // Make a file name which will created
    string nameOfFile;
    findTheFileNameInGivenPath(filePath, &nameOfFile); // find The File Name In Given Path

    // Make a path for file which will created
    string pathOfTheFileToBeCopied;
    mergethePathAndFileName(targetPath, nameOfFile , &pathOfTheFileToBeCopied); // Merge the target path And name Of file

    // Open the target file
    int dest = open(pathOfTheFileToBeCopied.c_str(), O_WRONLY, 0644);

    // if the file name already exist
    if(dest > 0){

         switch(ifTheFileAlreadyExists){
            case IF_THE_FILE_ALREADY_EXISTS_DO_NOTHING  :
                    return IF_THE_FILE_ALREADY_EXISTS_DO_NOTHING;

            case IF_THE_FILE_ALREADY_EXISTS_CHANGE_FILE_NAME  :
                {
                    // close dest
                    close(dest);

                    // find a unique name
                    findTheUniqueNameInThePath(&pathOfTheFileToBeCopied);

                    // find The File Name In Given Path
                    findTheFileNameInGivenPath(pathOfTheFileToBeCopied, &nameOfFile);

                    // Open the target file
                    int dest = open(pathOfTheFileToBeCopied.c_str(), O_WRONLY | O_CREAT, 0644);

                    // Copy file
                    if(!copyFromSourceFileToDestinationFile(source,dest))
                        return FAIL_COPY_FILE;  //if copy file fails

                    // make the file name which will created
                    theNameOfTheCopyFileCreated->clear();
                    theNameOfTheCopyFileCreated->append(nameOfFile);
                    return  IF_THE_FILE_ALREADY_EXISTS_CHANGE_FILE_NAME;
                    break;
                }

            case IF_THE_FILE_ALREADY_EXISTS_OVERWRITE :
                {
                    // Copy file
                    if(!copyFromSourceFileToDestinationFile(source,dest))
                        return FAIL_COPY_FILE;  //if copy file fails

                    // make the file name which will created
                    theNameOfTheCopyFileCreated->clear();
                    theNameOfTheCopyFileCreated->append(nameOfFile);
                    return IF_THE_FILE_ALREADY_EXISTS_OVERWRITE;
                   break;
                }

            default : // wrong ifTheFileAlreadyExists
                return FAIL_COPY_FILE;
         }

    }
    else // if the target file doesn't exist. then create it.
    {
        // Create the file
        dest = open(pathOfTheFileToBeCopied.c_str(), O_WRONLY | O_CREAT, 0644);

        // Copy file
        if(!copyFromSourceFileToDestinationFile(source,dest))
            return FAIL_COPY_FILE;  //if copy file fails

        // make the file name which will created
        theNameOfTheCopyFileCreated->clear();
        theNameOfTheCopyFileCreated->append(nameOfFile);
    }

    // if the created file was deleted during the process then return CREATED_FILE_DELETED_DURING_COPY_PROCESS.
    if(!isItAFile(pathOfTheFileToBeCopied))
        return CREATED_FILE_DELETED_DURING_COPY_PROCESS;

    // if everything is OK.
    return SUCCESS_COPY_FILE;
}

/*
 * Is given file path a file name
 *
 * @param filePath will checked.
 * @return filePath is a file name then return true. otherwise return false.
 */
bool GTUVOS::isItAFile(string filePath){

    FILE *filePtrInputFile = fopen(filePath.c_str(),"r"); /* Open File */

    /* If filePath didn't a file name */
    if (!filePtrInputFile)
        return false; // filePath is not a file name

    /* Close File */
    fclose(filePtrInputFile);

    return true;// filePath is a file name
}

/*
 * Is given directory path a directory name
 *
 * @param dirPath will checked.
 * @return dirPath is a directory name then return true. otherwise return false.
 */
bool GTUVOS::isItADirectory(string dirPath){

    DIR* dirPtrInputDirectory = opendir (dirPath.c_str()); /* Open Directory */

    /* If dirPath didn't a directory name */
    if (!dirPtrInputDirectory)
        return false; // dirPath is not a directory name

    /* Close directory */
    closedir(dirPtrInputDirectory);

    return true;// dirPath is a directory name
}

/*
 * Find The File Name In Given Path
 *
 * @param filePath need for search
 * @param theNameOfTheFileToBeCopied will be file name.
 */
void GTUVOS::findTheFileNameInGivenPath(string filePath, string* theNameOfTheFileToBeCopied){

    int indexOfBeginingOfTheFileName = 0;

    for(int i=0; i<filePath.length() ;++i)
       if(filePath[i] == '/')
           indexOfBeginingOfTheFileName = i + 1;

    // make the file name
    theNameOfTheFileToBeCopied->clear();
    theNameOfTheFileToBeCopied->append(filePath.substr(indexOfBeginingOfTheFileName));

}

/*
 * Merge The Given Path And Given File Name
 *
 * @param Path is beginning of the newPath
 * @param fileName is ending of the newPath
 * @param newPath will created.
 */
void GTUVOS::mergethePathAndFileName(string Path, string fileName, string* newPath){

    // clear newPath
    newPath->clear();

    // Append path
    newPath->append(Path);

    // If it isn't exist then append "/"
    if(Path[Path.length()-1] != '/')
        newPath->append("/");

    // Append the file name
    newPath->append(fileName);

}

/*
 * Copy From Source File To Destination File
 *
 * @param sourceFile is source file for copy process
 * @param destinationFile is destination file for copy process
 * @return copy fails then return false otherwise return true.
 */
bool GTUVOS::copyFromSourceFileToDestinationFile(int sourceFile, int destinationFile){

    // Struct required, rationale: function stat() exists also
    struct stat stat_source;
    fstat(sourceFile, &stat_source);
    int a = sendfile(destinationFile, sourceFile, 0, stat_source.st_size);

    // if something wrong about send File
    if ( a != stat_source.st_size)
        return false;

    // close source and dest
    close(sourceFile);
    close(destinationFile);

    return true;
}

/*
 * Find The Unique Name In The Path
 *
 * @param filePath is source path for finding process
 * @param newFileName will created.
 * @return is not important
 */
bool GTUVOS::findTheUniqueNameInThePath(string* filePath){

    int intValue = 0;
    int indexOfBeginingOfTheFileName = 0;
    string tempFilePath;
    tempFilePath.append(*filePath);

    //find the beginning of the file
    for(int i=0; i<tempFilePath.length() ;++i)
        if(tempFilePath[i] == '/')
            indexOfBeginingOfTheFileName = i + 1;

    // loop for finding unique name
    while(true){

        FILE *filePtrInputFile = fopen(filePath->c_str(),"r"); /* Open File */

        /* If filePath didn't a file name */
        if (!filePtrInputFile){
            return true; // end of the loop
        }

        /* Close File */
        fclose(filePtrInputFile);

        // try a new name
        ++intValue;                                                             // increase intValue
        filePath->clear();                                                      // clear file Path
        filePath->append(tempFilePath.substr(0,indexOfBeginingOfTheFileName));  // append begin of tempFilePath
        filePath->append("(");                                                  // append "("
        filePath->append(to_string(intValue));                                  // append intValue
        filePath->append(")");                                                  // append ")"
        filePath->append(tempFilePath.substr(indexOfBeginingOfTheFileName));    // append end of tempFilePath
    }
}
