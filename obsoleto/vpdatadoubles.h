#ifndef VPDATADOUBLES_H
#define VPDATADOUBLES_H

#include <QVector>
class vpDataDoubles
{
public:
    vpDataDoubles();
    vpDataDoubles(QVector<qreal>& row);
    QVector<qreal>& data() {return m_row;}
protected:
    QVector<qreal> m_row;
};

#endif // VPDATADOUBLES_H
