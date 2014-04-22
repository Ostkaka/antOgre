:: Builds the dependencies for ogre in VS2013

cd dependencies
cmake . -G "Visual Studio 12 Win64" -DCMAKE_CXX_FLAGS_RELWITHDEBINFO="/MD /Zi /Od" 

call setup_vs.bat

:: build the solution
devenv antDependencies.sln /BUILD Debug /project ogreDeps /projectconfig "Debug|x64" 
devenv antDependencies.sln /BUILD Debug /project ogre /projectconfig "Debug|x64" 

devenv antDependencies.sln /BUILD Release /project ogreDeps /projectconfig "Release|x64" 
devenv antDependencies.sln /BUILD Release /project ogre /projectconfig "Release|x64" 
