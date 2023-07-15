# Sound Agent
- an expiremental enthusiast-made version of soundpad
## build
- the project is made with 2 languages and 2 build systems respectively
> c++ with cmake
> c# with dotnet
### cmake and cplusplus
- the build process is classic cmake-like
> "cd ./gen"
> "cmake ../"
> "cmake --build ."
### dotnet and csharp
- the build process is a bit more complicated for dotnet
> first - we need bass library
> > download the library at [the bass project website](https://www.un4seen.com)
> > build the library from downloaded sources for a specific architecture
> > copy the result into the gen/bin/path/to/the/final/output
> then dotnet run and we are done
