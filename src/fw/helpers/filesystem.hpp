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
        }
    }
}

QString Fw::Helpers::FileSystem::pluginsDir()
{
    return QDir::toNativeSeparators(QCoreApplication::applicationDirPath()+ "/plugins/");
}

#endif // FILESYSTEM_HPP
