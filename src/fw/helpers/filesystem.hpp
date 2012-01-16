#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <QtCore/qdir.h>
#include <QtCore/qcoreapplication.h>

#include "fw/helpers/defs.hpp"

namespace Fw
{
    namespace Helpers
    {
        namespace FileSystem
        {
            inline QString pluginsDir();
            inline QString pluginFileName(const QString& name);
        }
    }
}

QString Fw::Helpers::FileSystem::pluginsDir()
{
    return QDir::toNativeSeparators(QCoreApplication::applicationDirPath()+ "/plugins/");
}

QString Fw::Helpers::FileSystem::pluginFileName(const QString& name)
{
    return QDir::toNativeSeparators(pluginsDir()
                                    + "lib" + name.toLower().simplified()
                                #if defined(Q_OS_LINUX)
                                    + ".so"
                                #elif defined(Q_OS_WINDOWS)
                                    + ".dll"
                                #elif defined(Q_OS_MAC)
                                    + ".dylib"
                                #else
                                    #error This os is not supported
                                #endif
                                    );
}


#endif // FILESYSTEM_HPP
