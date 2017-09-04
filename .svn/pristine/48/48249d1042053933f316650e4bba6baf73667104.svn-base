#ifndef SEARCHMARC_H
#define SEARCHMARC_H

#include <QThread>
#include "../../Common/LxTsspCommonDefinition.h"
#include "platformresource.h"

class SearchMarc : public QThread
{
    Q_OBJECT
public:
    explicit SearchMarc(QObject *parent = 0);
    ~SearchMarc();
    virtual void run();
    void setStop();
    void pause(){m_pause = true;}
    void continu(){m_pause = false;}

    bool init(PlatformResource *pRes);
signals:

public slots:
private:
    PlatformResource* m_pRes;
    bool m_bStop;
    bool m_pause;

};

#endif // SEARCHMARC_H
