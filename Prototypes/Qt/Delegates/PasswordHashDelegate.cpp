#include "PasswordHashDelegate.h"

#include <QCryptographicHash>
#include <QLineEdit>
#include <QPainter>

PasswordHashDelegate::PasswordHashDelegate()
{

}

void PasswordHashDelegate::setEditorData(QWidget* editor, const QModelIndex&) const
{
	if (auto line = qobject_cast<QLineEdit*>(editor)) {
		line->clear();
	}
}

void PasswordHashDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	if (auto line = qobject_cast<QLineEdit*>(editor)) {
		model->setData( index, QCryptographicHash::hash(line->text().toUtf8(), QCryptographicHash::Sha256) );
	}
}

void PasswordHashDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto data = index.data().toByteArray().toHex();

	painter->save();
	painter->setClipRect(option.rect);

	drawBackground(painter, option, index);
	drawDisplay(painter, option, option.rect, data);
	drawFocus(painter, option, option.rect);

	painter->restore();
}
