#ifndef Event_FilterAlg_H
#define Event_FilterAlg_H

#include <iostream>

#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StreamBuffer.h"
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"

#include "Event/TopLevel/Definitions.h"
#include "Event/TopLevel/EventModel.h"

/**
 * @class FilterAlgData
 *
 * @brief TDS for storing an event in a format similar to ebf
 *
 * The data is stored as one continuos string of bytes
 * No attempt is made to verify that the data stored is correctly
 * formated ebf.
 * $Header: /nfs/slac/g/glast/ground/cvs/OnboardFilter/OnboardFilter/FilterAlgTds.h,v 1.5 2005/09/02 19:08:11 hughes Exp $
 */

//extern const CLID& CLID_FilterAlgTds;

namespace FilterAlgTds{
  class FilterAlgData : public DataObject{
  public:
    FilterAlgData() : DataObject() { }
    virtual ~FilterAlgData() { };
    
    inline int getVetoWord() const;
    inline void setVetoWord(const int vetoword);
    
    inline virtual std::ostream& fillStream(std::ostream &s) const;
    inline friend std::ostream& operator << (std::ostream &s, const FilterAlgData& obj);
  private:   
    int m_vetoword;
  };

  //inline stuff for client
  
  inline int FilterAlgData::getVetoWord() const{
    return m_vetoword;
  }
  
  inline void FilterAlgData::setVetoWord(const int vetoword){
    m_vetoword=vetoword;
  }
  
  inline std::ostream& FilterAlgData::fillStream( std::ostream &s) const{
    //if(m_length>0)
    //  s.write(m_data,m_length);
    return s;
  }
  
  
  inline std::ostream& operator << (std::ostream& s, const FilterAlgData& obj){
    return obj.fillStream(s);
  }
  
}// namespace
#endif
