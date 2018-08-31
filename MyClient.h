#ifndef MYCLIENT_H
#define MYCLIENT_H
#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QMainWindow>
#include <QKeyEvent>
#include <QPushButton>

class QTextEdit;
class QLineEdit;

// ======================================================================
class MyClient : public QWidget{
Q_OBJECT
private:
    QTcpSocket* m_pTcpSocket;
    QTextEdit*  m_ptxtInfo;
    QLineEdit*  m_ptxtInput;
    quint16     m_nNextBlockSize;
    QPushButton* pcmd;
    int key;
    QString userName;

public:
    MyClient(const QString& strHost, int nPort, QWidget* pwgt = 0) ;
signals:
    void exitProgram();

private slots:
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer(                            );
    void slotConnected   (                            );
};

#endif // MYCLIENT_H
