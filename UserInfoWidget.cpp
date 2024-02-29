#include "UserInfoWidget.h"
#include "Util.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QGridLayout>

UserInfoWidget::UserInfoWidget(
    QWidget *parent
    )
    : QWidget{parent}
{
    this->initUI(this);
    connect(this->uploadAvatarBtn_,&QPushButton::clicked,this,&UserInfoWidget::uploadAvatarButtonClicked);
    connect(this->saveBtn_,&QPushButton::clicked,this,&UserInfoWidget::saveBtnClicked);
    connect(this->cancelBtn_,&QPushButton::clicked,this,&UserInfoWidget::cancelBtnClicked);
}

int UserInfoWidget::init(const QString &nickname, const QString &signatrue, const QPixmap &avatar)
{
    this->avatarLbl_->setPixmap(avatar);
    this->nicknameLdt_->setPlaceholderText(nickname);
    this->nicknameLdt_->setText("");
    this->signatureLdt_->setPlaceholderText(signatrue);
    this->signatureLdt_->setText("");
    this->avatarChanged_ = false;
    return 0;
}

int UserInfoWidget::setSaveBtnClickedCallback(std::function<void (UserInfoWidget &)> saveBtnClickedCallback)
{
    this->saveBtnClickedCallback_ = saveBtnClickedCallback;
    return 0;
}

QString UserInfoWidget::getNickname()
{
    return this->nicknameLdt_->text();
}

QString UserInfoWidget::getSignatrue()
{
    return this->signatureLdt_->text();
}

QPixmap UserInfoWidget::getAvatar()
{
    return this->avatarLbl_->pixmap();
}

bool UserInfoWidget::getAvatarChanged()
{
    return this->avatarChanged_;
}

void UserInfoWidget::initUI(QWidget *parent)
{
    ObjGuard g(8);
    this->avatarLbl_ = CreateQWidget<QLabel>(g,"UserInfoWidget_avatarLbl",parent);
    this->avatarLbl_->setScaledContents(true);
    this->uploadAvatarBtn_ = CreateQWidget<QPushButton>(g,"",parent);
    this->uploadAvatarBtn_->setText("修改头像");
    this->nicknameLbl_ = CreateQWidget<QLabel>(g,"",parent);
    this->nicknameLbl_->setText("昵称");
    this->nicknameLdt_ = CreateQWidget<QLineEdit>(g,"",parent);
    this->nicknameLdt_->setPlaceholderText("昵称");
    this->signatrueLbl_ = CreateQWidget<QLabel>(g,"",parent);
    this->signatrueLbl_->setText("个性签名");
    this->signatureLdt_ = CreateQWidget<QLineEdit>(g,"",parent);
    this->signatureLdt_->setPlaceholderText("个性签名");
    this->saveBtn_ = CreateQWidget<QPushButton>(g,"",parent);
    this->saveBtn_->setText("保存");
    this->cancelBtn_ = CreateQWidget<QPushButton>(g,"",parent);
    this->cancelBtn_->setText("取消");

    auto layout = CreateQObject<QGridLayout>(g,"",parent);
    layout->addWidget(this->avatarLbl_,0,0);
    layout->addWidget(this->uploadAvatarBtn_,0,1);
    layout->addWidget(this->nicknameLbl_,1,0);
    layout->addWidget(this->nicknameLdt_,1,1);
    layout->addWidget(this->signatrueLbl_,2,0);
    layout->addWidget(this->signatureLdt_,2,1);
    layout->addWidget(this->saveBtn_,3,0);
    layout->addWidget(this->cancelBtn_,3,1);
    return;
}

void UserInfoWidget::uploadAvatarButtonClicked(bool)
{
    QFileDialog f;
    if(!f.exec()){
        return ;
    }
    auto fileNames = f.selectedFiles();
    if(fileNames.count() == 0){
        return ;
    }
    const QString& fileName = fileNames[0];
    QPixmap pixmap;
    bool ok = pixmap.load(fileName);
    if(!ok){
        QMessageBox::information(
            this,
            tr("打开图像失败"),
            tr("打开图像失败!"));
        return;
    }
    this->avatarChanged_ = true;
    this->avatarLbl_->setPixmap(pixmap);
}

void UserInfoWidget::saveBtnClicked(bool)
{
    qDebug("saveBtnClicked");
    if(this->saveBtnClickedCallback_!=nullptr){
        this->saveBtnClickedCallback_(*this);
    }
    this->close();
    return ;
}

void UserInfoWidget::cancelBtnClicked(bool)
{
    qDebug("cancelBtnClicked");
    this->close();
}
