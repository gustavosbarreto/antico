#ifndef _DECORATIONSTYLELOADER_HPP
#define _DECORATIONSTYLELOADER_HPP

#include <QString>
#include <QPixmap>
#include <QList>

#include "styleabledecoration.hpp"

class DecorationStyleLoader
{
public:
    DecorationStyleLoader(const QString &file);
    DecorationStyleLoader(const DecorationStyleLoader &other);

    inline bool isValid() const { return _valid; }

    const QString styleSheet() const;

    inline const QString &authorName() const { return _authorName; }
    inline const QString &authorMail() const { return _authorMail; }
    inline const QString &styleName() const { return _styleName; }

    static QList<DecorationStyleLoader> styles();

private:
    bool loadInfo(const QString &path);

private:
    bool _valid;

    QString _styleRootPath;
    QString _authorName;
    QString _authorMail;
    QString _styleName;
};

#endif
