#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QComboBox>

class MyComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MyComboBox(QWidget *parent = nullptr);

    int getoldIndex() {return oldIndex;}
    void setoldIndex(int x) {oldIndex=x;}

signals:

public slots:

protected:
      void mousePressEvent(QMouseEvent *e)//实现保存改变前页码
      {
         oldIndex = this->currentIndex();
         QComboBox::mousePressEvent(e);
      }

private:
    int oldIndex;
};

#endif // MYCOMBOBOX_H
