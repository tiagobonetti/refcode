from distutils.core import setup, Extension

module1 = Extension('cppdemo',
                    sources = ['pysystem.cpp'],
                    extra_compile_args=['-std=c++11'])

setup (name = 'cppdemo',
       version = '0.1',
       description = 'This is a demo package',
       ext_modules = [module1])

