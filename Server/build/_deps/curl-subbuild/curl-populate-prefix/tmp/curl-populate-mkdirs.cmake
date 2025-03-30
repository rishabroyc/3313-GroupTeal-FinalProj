# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/_deps/curl-src"
  "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/_deps/curl-build"
  "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/_deps/curl-subbuild/curl-populate-prefix"
  "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/_deps/curl-subbuild/curl-populate-prefix/tmp"
  "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/_deps/curl-subbuild/curl-populate-prefix/src/curl-populate-stamp"
  "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/_deps/curl-subbuild/curl-populate-prefix/src"
  "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/_deps/curl-subbuild/curl-populate-prefix/src/curl-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/_deps/curl-subbuild/curl-populate-prefix/src/curl-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/mnt/c/Users/risha/OneDrive - The University of Western Ontario/Year 3/3313/FinalProject/3313-GroupTeal-FinalProj/Server/build/_deps/curl-subbuild/curl-populate-prefix/src/curl-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
