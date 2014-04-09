Eclipse Build of MongoType 2.4 dtd 20140409:

12:56:45 **** Clean-only build of configuration Debug for project MongoType ****
make clean 
rm -rf  ./src/BSONObjectParser.o ./src/BSONObjectTypeDump.o ./src/BSONTypeFormatter.o ./src/BSONTypeMap.o ./src/Parameters.o ./src/mongotype.o  ./src/BSONObjectParser.d ./src/BSONObjectTypeDump.d ./src/BSONTypeFormatter.d ./src/BSONTypeMap.d ./src/Parameters.d ./src/mongotype.d  mongotype
 
12:57:07 **** Build of configuration Debug for project MongoType ****
make all 
Building file: ../src/BSONObjectParser.cpp
Invoking: GCC C++ Compiler
g++ -I"../include" -I"/home/mdeazley/dev/boost/boost_1_55_0/include" -I"/opt/mongodb/mongo-cxx-driver/include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -pthread -Wno-deprecated-declarations -MMD -MP -MF"src/BSONObjectParser.d" -MT"src/BSONObjectParser.d" -o "src/BSONObjectParser.o" "../src/BSONObjectParser.cpp"
Finished building: ../src/BSONObjectParser.cpp
 
Building file: ../src/BSONObjectTypeDump.cpp
Invoking: GCC C++ Compiler
g++ -I"../include" -I"/home/mdeazley/dev/boost/boost_1_55_0/include" -I"/opt/mongodb/mongo-cxx-driver/include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -pthread -Wno-deprecated-declarations -MMD -MP -MF"src/BSONObjectTypeDump.d" -MT"src/BSONObjectTypeDump.d" -o "src/BSONObjectTypeDump.o" "../src/BSONObjectTypeDump.cpp"
Finished building: ../src/BSONObjectTypeDump.cpp
 
Building file: ../src/BSONTypeFormatter.cpp
Invoking: GCC C++ Compiler
g++ -I"../include" -I"/home/mdeazley/dev/boost/boost_1_55_0/include" -I"/opt/mongodb/mongo-cxx-driver/include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -pthread -Wno-deprecated-declarations -MMD -MP -MF"src/BSONTypeFormatter.d" -MT"src/BSONTypeFormatter.d" -o "src/BSONTypeFormatter.o" "../src/BSONTypeFormatter.cpp"
Finished building: ../src/BSONTypeFormatter.cpp
 
Building file: ../src/BSONTypeMap.cpp
Invoking: GCC C++ Compiler
g++ -I"../include" -I"/home/mdeazley/dev/boost/boost_1_55_0/include" -I"/opt/mongodb/mongo-cxx-driver/include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -pthread -Wno-deprecated-declarations -MMD -MP -MF"src/BSONTypeMap.d" -MT"src/BSONTypeMap.d" -o "src/BSONTypeMap.o" "../src/BSONTypeMap.cpp"
Finished building: ../src/BSONTypeMap.cpp
 
Building file: ../src/Parameters.cpp
Invoking: GCC C++ Compiler
g++ -I"../include" -I"/home/mdeazley/dev/boost/boost_1_55_0/include" -I"/opt/mongodb/mongo-cxx-driver/include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -pthread -Wno-deprecated-declarations -MMD -MP -MF"src/Parameters.d" -MT"src/Parameters.d" -o "src/Parameters.o" "../src/Parameters.cpp"
Finished building: ../src/Parameters.cpp
 
Building file: ../src/mongotype.cpp
Invoking: GCC C++ Compiler
g++ -I"../include" -I"/home/mdeazley/dev/boost/boost_1_55_0/include" -I"/opt/mongodb/mongo-cxx-driver/include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -pthread -Wno-deprecated-declarations -MMD -MP -MF"src/mongotype.d" -MT"src/mongotype.d" -o "src/mongotype.o" "../src/mongotype.cpp"
Finished building: ../src/mongotype.cpp
 
Building target: mongotype
Invoking: GCC C++ Linker
g++ -L"/opt/mongodb/mongo-cxx-driver/lib" -L"/home/mdeazley/dev/boost/boost_1_55_0/lib" -o "mongotype"  ./src/BSONObjectParser.o ./src/BSONObjectTypeDump.o ./src/BSONTypeFormatter.o ./src/BSONTypeMap.o ./src/Parameters.o ./src/mongotype.o   -lmongoclient -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system -lpthread
Finished building target: mongotype
 

12:57:28 Build Finished (took 21s.789ms)

 