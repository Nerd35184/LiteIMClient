#include "SearchResultItemWidget.h"
#include "Util.h"
#include <QGridLayout>


SearchResultItemWidget::SearchResultItemWidget(
    QWidget *parent,
    const QString &userId,
    const QPixmap &avatar,
    const QString &nickname,
    const QString &signature,
    std::function<void(SearchResultItemWidget& w)> callback):
    QWidget{parent},
    userId_(userId), addContantBtnClickedCallback_(callback)
{
    this->initUI(this);
    this->avatarLabel_->setPixmap(avatar);
    this->nicknameLabel_->setText(nickname);
    this->signatureLabel_->setText(signature);

    connect(this->addContantBtn_, &QPushButton::clicked, this, &SearchResultItemWidget::addContantBtnClicked);
    return;
}

const QString &SearchResultItemWidget::getUserIdR() const
{
    return this->userId_;
}

int SearchResultItemWidget::setAvatar(const QPixmap &avatar)
{
    this->avatarLabel_->setPixmap(avatar);
    return 0;
}

int SearchResultItemWidget::setNickname(const QString &nickanme)
{
    this->nicknameLabel_->setText(nickanme);
    return 0;
}

int SearchResultItemWidget::setSignatrue(const QString &signatrue)
{
    this->signatureLabel_->setText(signatrue);
    return 0;
}

int SearchResultItemWidget::setAddContantBtnClickedCallback(std::function<void(SearchResultItemWidget& w)> callback)
{
    this->addContantBtnClickedCallback_ = callback;
    return 0;
}

int SearchResultItemWidget::setBtnDisable()
{
    this->addContantBtn_->setText("已经申请");
    this->addContantBtn_->setDisabled(true);
    return 0;
}

void SearchResultItemWidget::addContantBtnClicked(bool)
{
    if (this->addContantBtnClickedCallback_ == nullptr)
    {
        return;
    }
    this->addContantBtnClickedCallback_(*this);
}

void SearchResultItemWidget::initUI(QWidget *parent)
{
    ObjGuard g(4);
    this->avatarLabel_ = CreateQWidget<QLabel>(g, "SearchResultItemWidget_avatarLbl", parent);
    this->avatarLabel_->setScaledContents(true);
    this->nicknameLabel_ = CreateQWidget<QLabel>(g, "SearchResultItemWidget_nicknameLbl", parent);
    this->signatureLabel_ = CreateQWidget<QLabel>(g, "SearchResultItemWidget_signatureLbl", parent);
    this->addContantBtn_ = CreateQWidget<QPushButton>(g, "SearchResultItemWidget_addContantBtn", parent);
    this->addContantBtn_->setText("添加好友");
    auto layout = CreateQLayout<QGridLayout>(parent);
    layout->addWidget(this->avatarLabel_, 0, 0, 2, 1);
    layout->addWidget(this->nicknameLabel_, 0, 1, 1, 1);
    layout->addWidget(this->signatureLabel_, 1, 1, 1, 1);
    layout->addWidget(this->addContantBtn_, 0, 2, 2, 1);
    return;
}

