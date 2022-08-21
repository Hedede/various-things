#ifndef NONEDITABLEDELEGATE_H
#define NONEDITABLEDELEGATE_H

#include <QItemDelegate>

class NonEditableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit NonEditableDelegate(QObject *parent = nullptr)
	: QItemDelegate(parent)
    {}

protected:
    bool editorEvent(QEvent*, QAbstractItemModel*, const QStyleOptionViewItem&, const QModelIndex&)
    { return false; }
    QWidget* createEditor(QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const
    { return Q_NULLPTR; }

};

#endif // NONEDITABLEDELEGATE_H
