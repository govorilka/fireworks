#ifndef FIREWORKS_HELPERS_URL_HPP
#define FIREWORKS_HELPERS_URL_HPP

#include <Qt/qurl.h>
#include <Qt/qfileinfo.h>

#include "fw/defs.hpp"
#include "fw/helpers/defs.hpp"

namespace Fw
{
    namespace Helpers
    {
        namespace Url
        {
            FIREWORKSSHARED_EXPORT inline QString Path(const QUrl& url);
        }
    }
}

QString Fw::Helpers::Url::Path(const QUrl& url)
{
    QUrl pathUrl(url);
    pathUrl.setPath(QFileInfo(url.path()).path());
    return pathUrl.toString() + '/';
}

#endif // FIREWORKS_HELPERS_URL_HPP
