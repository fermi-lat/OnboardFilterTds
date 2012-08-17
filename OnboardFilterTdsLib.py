# $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/OnboardFilterTds/OnboardFilterTdsLib.py,v 1.4 2012/04/26 21:48:45 jrb Exp $
def generate(env, **kw):
    if not kw.get('depsOnly', 0):
        env.Tool('addLibrary', library = ['OnboardFilterTds'])
        if env['PLATFORM']=='win32' and env.get('CONTAINERNAME','')=='GlastRelease':
	    env.Tool('findPkgPath', package='OnboardFilterTds')
    if env['PLATFORM']=='win32' and env.get('CONTAINERNAME','')=='GlastRelease':
        env.Tool('findPkgPath', package = 'enums') 
 
    env.Tool('EventLib')
    env.Tool('addLibrary', library=env['obfLibs'])
def exists(env):
    return 1;
