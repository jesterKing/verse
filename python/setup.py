from distutils.core import setup, Extension

verse_mod = Extension('verse',
        sources = ['verse_module.c'],
        include_dirs = ['../include'],
        libraries = ['kernel32', 'user32', 'ws2_32', 'verse_st', 'winstuff',
                        'ssleay32MD', 'libeay32MD', 'pthreadVC2'],
        library_dirs = ['../buildattempts/lib/Debug', 'D:/OpenSSL-Win64/lib/VC',
                        'D:/blenderdev/lib/win64/pthreads/lib'],
        extra_link_args = ['/NODEFAULTLIB:libcmt.lib', '/NODEFAULTLIB:libcmtd.lib'])


setup(name = "verse",
    version = "1.0",
    description = "The verse extension module",
    ext_modules = [verse_mod],
)

