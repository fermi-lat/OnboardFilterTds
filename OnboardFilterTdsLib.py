# $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/OnboardFilterTds/OnboardFilterTdsLib.py,v 1.3 2009/01/23 00:21:00 ecephas Exp $
def generate(env, **kw):
    if not kw.get('depsOnly', 0):
        env.Tool('addLibrary', library = ['OnboardFilterTds'])
    env.Tool('EventLib')
    env.Tool('addLibrary', library=env['obfLibs'])
def exists(env):
    return 1;
