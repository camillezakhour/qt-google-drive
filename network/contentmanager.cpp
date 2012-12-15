#include "contentmanager.h"
#include "share/commontools.h"
#include "share/debug.h"
#include <QApplication>

ContentManager::ContentManager(QObject *parent):
    NetworkManager(parent),
    opMngr(new OperationsManager(parent))
{
    parser.reset(new XMLParser);
}

ContentManager::~ContentManager()
{
}

void ContentManager::get(const QString &url)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    CommonTools::setHeader(request);
    getRequest(url);
}

void ContentManager::slotReplyFinished(QNetworkReply*)
{
    CommonTools::logToFile(QString("ParserReply ") + ".txt", replyStr.toAscii());

//    DEBUG << "<===============================================================================================================";
//    DEBUG << "replyStr" << replyStr;
//    DEBUG << "===============================================================================================================>";

    if(parseReply(replyStr)) DEBUG << "parse OK";
    else DEBUG << "parse not OK";

    replyStr.clear();

    if(!parser->getXMLHandler()->resDownloadingNow())
    {
        show();
    }

    QApplication::restoreOverrideCursor();
}

bool ContentManager::parseReply(const QString &str)
{
    QXmlSimpleReader reader;
    QXmlInputSource source;

    parser.reset(new XMLParser);

    connect(parser->getXMLHandler(), SIGNAL(signalAllResDownloaded(/*int*/)),this, SLOT(slotResDownloaded(/*int*/)));

    source.setData(str.toAscii());

    reader.setContentHandler(parser.data());
    reader.setErrorHandler(parser.data());

    return reader.parse(&source);
}

void ContentManager::slotResDownloaded(void)
{
    show();
}

XMLParser* ContentManager::getParser(void) const
{
    return parser.data();
}

void ContentManager::clear(void)
{
    treeWidgetItems.clear();

    if(treeWidgetItems.empty()) return;

    for(int i = 0; i < treeWidgetItems.count(); ++i)
    {
        delete treeWidgetItems[i];
    }
}

OperationsManager* ContentManager::getOpMngr(void) const
{
    return opMngr.data();
}



