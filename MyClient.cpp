#include <QtNetwork>
#include <QtGui>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include "MyClient.h"

// ----------------------------------------------------------------------
MyClient::MyClient(const QString& strHost,
                   int            nPort,
                   QWidget*       pwgt /*=0*/
                  ) : QWidget(pwgt)
                    , m_nNextBlockSize(0)
{
    QObject::connect(this, SIGNAL(exitProgram()),qApp, SLOT(quit()));
    bool ok;
    userName =  QInputDialog::getText(this, tr("Enter your name"),
    tr("Before enter the chat you must enter your name:"),
    QLineEdit::Normal,QDir::home().dirName(), &ok);
 if (ok && !userName.isEmpty())
 {
     qDebug()<<userName;

     //textLabel->setText(text);
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(strHost, nPort);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError))
           );

    m_ptxtInfo  = new QTextEdit;
    m_ptxtInput = new QLineEdit;

    connect(m_ptxtInput, SIGNAL(returnPressed()),
            this,        SLOT(slotSendToServer())
           );

    m_ptxtInfo->setReadOnly(true);

    pcmd = new QPushButton("&Send");
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;

    pvbxLayout->addWidget(new QLabel("<H1>Client "+userName+"</H1>"));
    pvbxLayout->addWidget(m_ptxtInfo);
    pvbxLayout->addWidget(m_ptxtInput);
    pvbxLayout->addWidget(pcmd);
    setLayout(pvbxLayout);

 }   else
 {
      qApp->exec();
 }

}

// ----------------------------------------------------------------------
void MyClient::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_7);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < (int)sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in>> time >> str;

        m_ptxtInfo->append(time.toString() + " " + str);
        m_nNextBlockSize = 0;
    }
}

// ----------------------------------------------------------------------
void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    m_ptxtInfo->append(strError);
}

// ----------------------------------------------------------------------
void MyClient::slotSendToServer()
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    if(m_ptxtInput->text() == "exit")
     emit exitProgram();
    else
    {
     QString message;
     message.append(userName);
     message.append(": ");
     message.append(m_ptxtInput->text());
    out << quint16(0) << QTime::currentTime()<< message;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    m_pTcpSocket->write(arrBlock);
    m_ptxtInput->setText("");
    }
}

// ------------------------------------------------------------------
void MyClient::slotConnected()
{
    m_ptxtInfo->append("Received the connected() signal");
}
