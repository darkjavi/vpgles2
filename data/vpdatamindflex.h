#ifndef VPDATAMINDFLEX_H
#define VPDATAMINDFLEX_H

#include "vpdatatimedarray.h"

class vpDataMindFlex : public vpDataTimedArray
{
public:
    vpDataMindFlex(QString id = "none");
    vpData* RawData() {return &m_rawData;}
protected:
    vpDataTimedArray m_rawData;
};

#endif // VPDATAMINDFLEX_H
