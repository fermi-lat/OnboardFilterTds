# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/OnboardFilterTds/SConscript,v 1.5 2009/08/08 01:03:59 jrb Exp $
# Authors: Tracy Usher <usher@SLAC.Stanford.edu>
# Version: OnboardFilterTds-00-10-01
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('OnboardFilterTdsLib', depsOnly = 1)

libEnv.AppendUnique(CPPDEFINES = ['GLEAM'])
libEnv.AppendUnique(CPPDEFINES = ['__i386'])
libEnv.AppendUnique(CPPDEFINES = ['EFC_FILTER'])

if baseEnv['PLATFORM'] == 'win32':
	libEnv.AppendUnique(CPPDEFINES = ['_WIN32'])
if baseEnv['PLATFORM'] == 'Linux':
	libEnv.AppendUnique(CPPDEFINES = ['MSG_ST'])
	libEnv.AppendUnique(CPPDEFINES = ['__unix__'])
	libEnv.AppendUnique(CPPDEFINES = ['WUT_IMPLEMENTATION_PX'])	

OnboardFilterTds = libEnv.SharedLibrary('OnboardFilterTds',
                                        listFiles(['src/*.cxx']))

progEnv.Tool('OnboardFilterTdsLib')

if baseEnv['PLATFORM'] == 'Linux':
	progEnv.Tool('addLibrary', library = ['dl'])
	progEnv.Tool('addLibrary', library = ['pthread'])

test_OnboardFilterTds = progEnv.Program('test_OnboardFilterTds', listFiles(['src/test/*.cxx']))
progEnv.Tool('registerTargets', package = 'OnboardFilterTds',
             libraryCxts = [[OnboardFilterTds, libEnv]], 
             testAppCxts = [[test_OnboardFilterTds, progEnv]], 
             includes = listFiles(['OnboardFilterTds/*'], recursive=True))




