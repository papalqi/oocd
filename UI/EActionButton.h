#ifndef EACTIONBUTTON_H
#define EACTIONBUTTON_H

#include <QAbstractButton>
#include <QtWin>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QtWinExtras/QtWinExtras>

class EFileTypPix
{
public:
    static EFileTypPix& instance()
    {
        if(!I) I = new EFileTypPix;
        return *I;
    }
    static void Release()
    {
        if(I)
        {
            delete I;
            I = nullptr;
        }
    }
    EFileTypPix()
    {
        //init();
    }
    QPixmap getPixFromTyp(QString typ)
    {
        QPixmap pixmap;
        QString type;
        int nPos = -1;
        nPos = typ.indexOf(".");
        if (nPos >= 0)
        {
            // Qt4：QPixmap::fromWinHICON(icon)
            pixmap = fileIcon(typ).pixmap(25,25);
            type = fileType(typ);
        }
        return pixmap;
//        if (strArray.indexOf(typ) != -1 && strArray.indexOf(typ) <= mFilePix.size())
//            return mFilePix[strArray.indexOf(typ)];
//        return NULL;
    }

private:
    void init()
    {
        strArray << "folder"<< ".exe"<< ".zip"<< ".har"<< ".hwl"<< ".accdb"<< \
                ".xlsx"<< ".pptx"<< ".docx"<< ".txt"<< ".h"<< ".cpp"<< ".pro";
        for (int i = 1; i < strArray.size() ; ++i)
        {
            // 获取图标、类型
            QPixmap pixmap;
            QString type;
            int nPos = -1;
            nPos = strArray[i].indexOf(".");
            if (nPos >= 0)
            {
                // Qt4：QPixmap::fromWinHICON(icon)
                pixmap = fileIcon(strArray[i]).pixmap(250,250);
                type = fileType(strArray[i]);
            }
            else
            {
//                pixmap = folderIcon().pixmap(50,50);
//                type = folderType();
            }
            mFilePix.append(pixmap);
        }
    }

    QIcon fileIcon(const QString &extension) const
    {
        QFileIconProvider provider;
        QIcon icon;
        QString strTemplateName = QDir::tempPath() + QDir::separator() +
                QCoreApplication::applicationName() + "_XXXXXX." + extension;
        QTemporaryFile tmpFile(strTemplateName);
        tmpFile.setAutoRemove(false);

        if (tmpFile.open())
        {
            tmpFile.close();
            icon = provider.icon(QFileInfo(tmpFile.fileName()));
            // tmpFile.remove();
        }
        else
        {
            qCritical() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
        }

        return icon;
    }

    QString fileType(const QString &extension) const
    {
        QFileIconProvider provider;
        QString strType;
        QString strFileName = QDir::tempPath() + QDir::separator() +
                QCoreApplication::applicationName() + "_XXXXXX." + extension;
        QTemporaryFile tmpFile(strFileName);
        tmpFile.setAutoRemove(false);

        if (tmpFile.open())
        {
            tmpFile.close();
            strType = provider.type(QFileInfo(tmpFile.fileName()));
            // tmpFile.remove();
        }
        else
        {
            qCritical() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
        }

        return strType;
    }

private:
    static EFileTypPix *I;

    QList<QPixmap> mFilePix;
    QStringList strArray;
};


class EActionButton: public QAbstractButton
{
    Q_OBJECT
public:
    EActionButton(QWidget* parent = 0);
    void setPixMapNormal(const QPixmap &pixMap);//普通图片
    void setPixMapPressed(const QPixmap &pixMap);//按下图片
    void setItemName(const QString &sName, const QString &spath);//按钮名称
    void setRadius(int nTopLeft,int nTopRight,int nBottomRight,int nBottomLeft);//圆角
    void setActionEnum();
    //EActionButtonEnum getActionEnum();
    void updateStyleSheet();//更新样式

    bool getChecked() const;
    void setChecked(bool checked);
signals:
    void sigClick(int sid);
private slots:
    void slot_click();
    void slot_Press();
    void slot_Relea();
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    void init();
    void initConnect();
private:
    QPixmap m_PixMapNormal;//普通图片
    QPixmap m_PixMapPressed;//选中图片
    QString m_sName;//名称
	QString m_sPath;

    int m_nTopLeftRadius;//左上圆角
    int m_nTopRightRadius;//右上圆角
    int m_nBottomRightRadius;//右下圆角
    int m_nBottomLeftRadius;//坐下圆角

    bool m_checked;//是否选中

    //缩放比例
    float mWFactor;
    float mHFactor;
    float mMinFactor;

};

#endif // EACTIONBUTTON_H
