#ifndef PASSWORDHASHDELEGATET_H
#define PASSWORDHASHDELEGATET_H

#include <QItemDelegate>

class PasswordHashDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	PasswordHashDelegate();


	// QAbstractItemDelegate interface
public:
	//QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // PASSWORDHASHDELEGATET_H
