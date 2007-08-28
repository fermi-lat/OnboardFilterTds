#ifndef FILTER_STATUS_H
#define FILTER_STATUS_H

#include <iostream>
#include <vector>

#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StreamBuffer.h"
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/IInterface.h"

#include "Event/TopLevel/Definitions.h"
#include "Event/TopLevel/EventModel.h"

//#include "TkrUtil/ITkrGeometrySvc.h"
#include "EDS/EBF_dir.h"
#include "EDS/EDR_tkr.h"
#include "EFC/GFC_status.h"
#include "EFC/TFC_prjDef.h"
#include "OnboardFilterTds/LogInfoDef.h"
/**
 * @class FilterStatus
 * @brief TDS for storing the information returned by the filter
 */
//extern const CLID& CLID_FilterStatus;
static const CLID& CLID_FilterStatus = InterfaceID("FilterStatus", 0, 0);

class OnboardFilter;


namespace OnboardFilterTds
{
    
class TowerHits : public DataObject
{
public:
    struct towerRecord{
        unsigned char lcnt[2];
        int           layerMaps[2];
        unsigned char cnt[36];
        TFC_hit*      beg[36];
    };
    TowerHits() : DataObject() {
        for(int counter=0;counter<16;counter++){
            m_hits[counter].lcnt[0]      = m_hits[counter].lcnt[1]      = 0;
            m_hits[counter].layerMaps[0] = m_hits[counter].layerMaps[1] = 0;
            for(int layerCounter=0;layerCounter<36;layerCounter++){
                m_hits[counter].cnt[layerCounter]=0;
                m_hits[counter].beg[layerCounter]=NULL;
            }
        }
    };
    virtual ~TowerHits(){
        for(int counter=0;counter<16;counter++){
            for(int layerCounter=0;layerCounter<36;layerCounter++){
                if(m_hits[counter].beg[layerCounter]){
                    delete[] m_hits[counter].beg[layerCounter];
                }
            }
        }
    };
    TowerHits(const TowerHits &toCopy){
        for(int counter=0;counter<16;counter++){
            m_hits[counter].lcnt[0]=toCopy.m_hits[counter].lcnt[0];
            m_hits[counter].lcnt[1]=toCopy.m_hits[counter].lcnt[1];
            m_hits[counter].layerMaps[0]=toCopy.m_hits[counter].layerMaps[0];
            m_hits[counter].layerMaps[1]=toCopy.m_hits[counter].layerMaps[1];
            for(int layerCounter=0;layerCounter<36;layerCounter++){
                m_hits[counter].cnt[layerCounter]=toCopy.m_hits[counter].cnt[layerCounter];
//                m_hits[counter].beg[layerCounter]=new short int[m_hits[counter].cnt[layerCounter]];
                m_hits[counter].beg[layerCounter]=new TFC_hit[m_hits[counter].cnt[layerCounter]];
                memcpy(m_hits[counter].beg[layerCounter],
                       toCopy.m_hits[counter].beg[layerCounter],m_hits[counter].cnt[layerCounter]*sizeof(TFC_hit));
            }
        }
    };
    inline const towerRecord* get(){
        return m_hits;
    };
    inline void set(towerRecord* hits){
        for(int counter=0;counter<16;counter++){
            m_hits[counter].lcnt[0]=hits[counter].lcnt[0];
            m_hits[counter].lcnt[1]=hits[counter].lcnt[1];
            m_hits[counter].layerMaps[0]=hits[counter].layerMaps[0];
            m_hits[counter].layerMaps[1]=hits[counter].layerMaps[1];
            for(int layerCounter=0;layerCounter<36;layerCounter++){
                m_hits[counter].cnt[layerCounter]=hits[counter].cnt[layerCounter];
                if(m_hits[counter].beg[layerCounter]){
                    delete[] m_hits[counter].beg[layerCounter];
                }
//                m_hits[counter].beg[layerCounter]=new short int[m_hits[counter].cnt[layerCounter]];
                m_hits[counter].beg[layerCounter]=new TFC_hit[m_hits[counter].cnt[layerCounter]];
                memcpy(m_hits[counter].beg[layerCounter],
                       hits[counter].beg[layerCounter],m_hits[counter].cnt[layerCounter]*sizeof(TFC_hit));
            }
        }
    };
  
    towerRecord m_hits[16];
};


class track
{
public:
    inline track(){};
    inline track(const track &copy){
        lowCoord=copy.lowCoord;
        highCoord=copy.highCoord;
        exLowCoord=copy.exLowCoord;
        exHighCoord=copy.exHighCoord;
        length=copy.length;
        phi_rad=copy.phi_rad;
        theta_rad=copy.theta_rad;
        numhits=copy.numhits;
    };
    int numhits;
    double phi_rad,theta_rad;
    std::vector<double> lowCoord;
    std::vector<double> highCoord;
    std::vector<double> exLowCoord;
    std::vector<double> exHighCoord;
    double length;
};


class FilterStatus : public DataObject
{
public:
    FilterStatus() : DataObject() {
        m_status            = 0;
        m_stageEnergy       = 0;
        m_tcids             = 0;
        m_ebfsize           = 0;

        // init GEM
        m_gem_thrTkr        = 0;
        m_gem_calHiLo       = 0;
        m_gem_condsumCno    = 0;
        m_gem_acd_vetoes_XZ = 0;
        m_gem_acd_vetoes_YZ = 0;
        m_gem_acd_vetoes_XY = 0;   
        m_gem_acd_vetoes_RU = 0;
        m_gem_livetime      = 0;
        m_gem_trgtime       = 0;
        m_gem_ppstime       = 0;
        m_gem_discarded     = 0;
        m_gem_prescaled     = 0;
        m_gem_sent          = 0;

        m_xHits             = 0;
        m_yHits             = 0;
        m_slopeXZ           = 0.0;
        m_slopeYZ           = 0.0;
        m_intXZ             = 0.0;
        m_intYZ             = 0.0;

        m_acd_xy            = 0;
        m_acd_xz            = 0;
        m_acd_yz            = 0;
        m_vetoword          = 0;

        m_numLogsHit        = 0;

        m_separation        = -1;
        m_tracks.clear();
  
        m_tmsk              = 0;

        //****TEMP
        m_separation2       = -1.;;
        m_x_report          =  0.;  
        m_y_report          =  0.;
        m_xavg              =  0.;
        m_yavg              =  0.;
        m_xslopeL           =  0.;
        m_yslopeL           =  0.;
        m_xslopeAvg         =  0.;
        m_yslopeAvg         =  0.;
        m_long_firstanglex  =  0.;
        m_long_firstangley  =  0.;
        m_xslopetower       =  0.;
        m_yslopetower       =  0.;
        m_xangleL           =  0.;
        m_yangleL           =  0.;
        m_xlongestB         =  0.;
        m_ylongestB         =  0.;
        m_highestlayer      =  0.;

        double m_houghsep   =  0.;
        //****TEMP

        // Zero the EDR_tkr structure
        int sizeofedrtkr = sizeof(EDR_tkr);
        m_tkr = (EDR_tkr*)(new char[sizeofedrtkr]);
        memset(m_tkr, 0, sizeof(EDR_tkr));
        //memset(&m_tkr, 0, sizeof(EDR_tkr));

        // Zero the TFC_prjs structure
        memset(&m_prjs, 0, sizeof(TFC_prjs));

        // zero log data
        memset(m_logData, 0, 16*8*12*sizeof(LogInfo));

        unsigned int counter;
        // Loop over towers
        for(counter=0;counter<16;counter++){
            m_acdStatus[counter] = 0;
            m_layers[counter]    = 0;
            m_xcapture[counter]  = 0;
            m_ycapture[counter]  = 0;
            m_xy00[counter]      = 0;
            m_xy11[counter]      = 0;
            m_xy22[counter]      = 0;
            m_xy33[counter]      = 0;
        }
        for (counter = 0; counter < 8; counter++) {
            m_layerEnergy[counter] = 0;
        }
    }

    virtual ~FilterStatus() { };

    virtual const CLID& clID() const { return FilterStatus::classID(); }
    static const CLID& classID() { return CLID_FilterStatus; }


    ///Return the statuscode of the filter
    inline unsigned int get() const;
    ///Returns the 16 most significant bits of the filter code
    inline unsigned int getHigh() const;
    ///Returns the 16 least signifcat bits of the filter code
    inline unsigned int getLow() const;
    ///Returns the value stored in CalEnergy
    inline float getCalEnergy() const;
    inline const int getNumLogsHit() const;
    inline const LogInfo* getLogInfo() const;
    int getEbfSize() const;
    int getStageEnergy() const;
    ///Return the Code specifying the towers with triggers or possible triggers
    int getTcids() const;
    ///Return the ACD hit map results
    inline void getAcdMap(int &xz, int &yz, int &xy) const;
    ///Return the ACD faces intersected by projections
    inline const int* getAcdStatus() const;
    ///Return pointer to array of layers that were hit in each tower
    inline const int *getLayers()const ;
    ///Return the projections for a specific tower
    //const projections *getProjection(int tower)const ;
    inline TFC_prjs *getProjections() ;
    inline EDR_tkr *getTkr() ;
    ///Return all available tracks
    inline std::vector<track> getTracks()const;
    ///Return angular separation between best track and incomming particle
    inline double getSeparation() const;
    inline bool tracksExist() const;
    inline const int* getXcapture() const;
    inline const int* getYcapture() const;
    ///return the Cal Layer Energy
    inline const float* getLayerEnergy() const;
    ///Return xy00
    inline const int* getXY00() const;
    ///Return xy11
    inline const int* getXY11() const;
    ///Return xy22
    inline const int* getXY22() const;
    ///Return xy33
    inline const int* getXY33() const;
    ///Return tmsk
    inline int getTmsk() const;
    inline int getVetoWord() const;
    inline void setVetoWord(const int vetoword);

    inline void setBestTrack( int xHits,int yHits,
                              double slopeXZ,double slopeYZ,
                              double intXZ,double intYZ){
        m_xHits    = xHits;
        m_yHits    = yHits;
        m_slopeXZ  = slopeXZ;
        m_slopeYZ  = slopeYZ;
        m_intXZ    = intXZ;
        m_intYZ    = intYZ;
      }

    inline void FilterStatus::getBestTrack(
                                  int &xHits,int &yHits,
                                  double &slopeXZ,double &slopeYZ,
                                  double &intXZ,double &intYZ){
        xHits    = m_xHits;
        yHits    = m_yHits;
        slopeXZ  = m_slopeXZ;
        slopeYZ  = m_slopeYZ;
        intXZ    = m_intXZ;
        intYZ    = m_intYZ;
      }

    inline virtual std::ostream& fillStream(std::ostream &s) const;
    inline friend std::ostream& operator << (std::ostream &s, const FilterStatus &obj);

    // only this guy can create one, or modify it
    friend class OnboardFilter;

    ///Set the statuscode of the filter
    inline void set(const unsigned int code);
    ///Set the Energy in CAL
    ///Set the Energy in CAL          
    inline void setStageEnergy(const int energy);
    inline void setEbfSize(const int ids);
    ///Set the Code specifying the towers with triggers or possible triggers
    inline void setTcids(const int ids);
    ///Set the ACD hit map results
    inline void setAcdMap(const int xz, const int yz, const int xy);
    ///Set the ACD faces intersected by projections
    inline void setAcdStatus(const int tower, const int status);
    ///Set what layers were hit in each tower
    inline void setLayers(const int *layerCode);
    ///Set the projection of a specific tower
    //void setProjection(const int tower,const projections &projections);
    inline void setProjections(const TFC_prjs &projections);
    inline void setTkr(const EDR_tkr &tkr);
    ///Add a new track to the list of tracks
    inline void setTrack(const track &newTrack);
    inline void setSeparation(const double sep);
    inline void setLayerEnergy(const int energy[8]);
    inline void setLogData(int num, LogInfo tempLogData[]);

    inline void setCapture(const int xcapture[16], const int ycapture[16]);
    inline void setXY(const int xy00[16], const int xy11[16], const int xy22[16], const int xy33[16]);
    inline void setTmsk(int tmsk);
    //****TEMP
    inline void setHoughSep(const double houghsep);
    inline void setHighLayer(const double highestlayer);

    inline void setSeparation2(const double sep2);
    inline void setXlongest(const double x_report);
    inline void setYlongest(const double y_report);
    inline void setXavg(const double xavg);
    inline void setYavg(const double yavg);
    inline void setXslopeL(const double xslopeL);
    inline void setYslopeL(const double yslopeL);
    inline void setXslopeAvg(const double xslopeAvg);
    inline void setYslopeAvg(const double yslopeAvg);
    inline void setXfirst(const double long_firstanglex);
    inline void setYfirst(const double long_firstangley);
    inline void setXtower(const double xslopetower);
    inline void setYtower(const double yslopetower);
    inline void setXangleL(const double xangleL);
    inline void setYangleL(const double yangleL);
    inline void setXlongestB(const double xlongestB);
    inline void setYlongestB(const double ylongestB);

    inline double getHoughSep() const;
    inline double getHighLayer() const;

    inline double getSeparation2() const;
    inline double getXlongest() const;
    inline double getYlongest() const;
    inline double getXavg() const;
    inline double getYavg() const;
    inline double getXslopeL() const;
    inline double getYslopeL() const;
    inline double getXslopeAvg() const;
    inline double getYslopeAvg() const;
    inline double getXfirst() const;
    inline double getYfirst() const;
    inline double getXtower() const;
    inline double getYtower() const;
    inline double getXangleL() const;
    inline double getYangleL() const;
    inline double getXlongestB() const;
    inline double getYlongestB() const;

    inline int getGemThrTkr()const;
   
    inline int getGemCalHiLo() const;
    inline int getGemCondsumCno() const;
    inline int getGemAcd_vetoes_XZ() const;
    inline int getGemAcd_vetoes_YZ() const;
    inline int getGemAcd_vetoes_XY() const;
    inline int getGemAcd_vetoes_RU() const;
    inline int getGemLivetime() const;
    inline int getGemTrgtime() const;
    inline int getGemPpstime() const;
    inline int getGemDiscarded() const;
    inline int getGemPrescaled() const;
    inline int getGemSent()const;

    
    inline void setGemThrTkr(const int val);
    inline void setGemCalHiLo(const int val);
    inline void setGemCondsumCno(const int val);
    inline void setGemAcd_vetoes_XZ(const int val);
    inline void setGemAcd_vetoes_YZ(const int val);
    inline void setGemAcd_vetoes_XY(const int val);
    inline void setGemAcd_vetoes_RU(const int val);
    inline void setGemLivetime(const int val);
    inline void setGemTrgtime(const int val);
    inline void setGemPpstime(const int val);
    inline void setGemDiscarded(const int val);
    inline void setGemPrescaled(const int val);
    inline void setGemSent(const int val);


    //****TEMP

private:
    ///Filter status code
    unsigned int m_status;
    ///Energy in CAL
    int m_stageEnergy;
    ///Towers with triggers
    int m_tcids;

   int m_ebfsize;
    ///Gem info
   int m_gem_thrTkr;  
   int m_gem_calHiLo;  
   int m_gem_condsumCno;
   int m_gem_acd_vetoes_XZ;
   int m_gem_acd_vetoes_YZ;
   int m_gem_acd_vetoes_XY;
   int m_gem_acd_vetoes_RU;
   int m_gem_livetime;
   int m_gem_trgtime;   
   int m_gem_ppstime;
   int m_gem_discarded; 
   int m_gem_prescaled; 
   int m_gem_sent;
    LogInfo m_logData[16*8*12];    // 16 towers * 8 layers * 12 logs

  // BestTRack info
   int m_xHits;
   int m_yHits;
   double m_slopeXZ;
   double m_slopeYZ;
   double m_intXZ;
   double m_intYZ;


   int m_numLogsHit;
    ///ACD hit map
    int m_acd_xz;
    int m_acd_yz;
    int m_acd_xy;
    int m_vetoword;
    ///ACD faces
    int m_acdStatus[16];
    //std::vector<int> m_acdStatus(16);
    ///Layers hit in each tower
    int m_layers[16];
    ///Projections for the towers
    //projections m_prjs[16];
    TFC_prjs m_prjs;
    EDR_tkr*         m_tkr;
    ///Tracks found for this event
    std::vector<track> m_tracks;
    ///Angular separation between best track and incomming particle
    double m_separation;

    float m_layerEnergy[8];
    int m_xcapture[16];
    int m_ycapture[16];
    int m_xy00[16],m_xy11[16],m_xy22[16],m_xy33[16];
    int m_tmsk;
    //****TEMP
    double m_separation2;
    double m_x_report;  double m_y_report;
    double m_xavg;      double m_yavg;
    double m_xslopeL;   double m_yslopeL;
    double m_xslopeAvg; double m_yslopeAvg;
    double m_long_firstanglex; double m_long_firstangley;
    double m_xslopetower; double m_yslopetower;
    double m_xangleL;    double m_yangleL;
    double m_xlongestB;  double m_ylongestB;
    double m_highestlayer;

    double m_houghsep;
    //****TEMP

  };

  // inline the public get methods for clients besides OnboardFilter.

  inline unsigned int FilterStatus::get() const{
    return m_status;
  }

  inline unsigned int FilterStatus::getHigh() const{
    return m_status>>15;
  }

  inline unsigned int FilterStatus::getLow() const{
    return m_status & 0x7FFF;
  }

   inline int FilterStatus::getGemThrTkr()const{
    return m_gem_thrTkr;
  }
   
   inline int FilterStatus::getGemCalHiLo()const{
    return m_gem_calHiLo;
  } 
   inline int FilterStatus::getGemCondsumCno()const{
    return m_gem_condsumCno;
  }
   inline int FilterStatus::getGemAcd_vetoes_XZ()const{
    return m_gem_acd_vetoes_XZ;
  }
   inline int FilterStatus::getGemAcd_vetoes_YZ()const{
    return m_gem_acd_vetoes_YZ;
  }
   inline int FilterStatus::getGemAcd_vetoes_XY()const{
    return m_gem_acd_vetoes_XY;
  }
   inline int FilterStatus::getGemAcd_vetoes_RU()const{
    return m_gem_acd_vetoes_RU;
  }
   inline int FilterStatus::getGemLivetime()const{
    return m_gem_livetime;
  }
   inline int FilterStatus::getGemTrgtime()const{
    return m_gem_trgtime;
  }  
   inline int FilterStatus::getGemPpstime()const{
    return m_gem_ppstime;
  }
   inline int FilterStatus::getGemDiscarded()const{
    return m_gem_discarded;
  } 
   inline int FilterStatus::getGemPrescaled()const{
    return m_gem_prescaled;
  } 
   inline int FilterStatus::getGemSent()const{
    return m_gem_sent;
  }


  inline void FilterStatus::getAcdMap(int &xz, int &yz, int &xy) const {
    xz=m_acd_xz;
    yz=m_acd_yz;
    xy=m_acd_xy;
  }

  inline const int* FilterStatus::getAcdStatus() const {
    return m_acdStatus;
  }

  inline TFC_prjs * FilterStatus::getProjections(){
    return &m_prjs;
  }
  inline EDR_tkr * FilterStatus::getTkr(){
//    return &m_tkr;
    return m_tkr;
  }
  inline float FilterStatus::getCalEnergy() const{
    return (float)((m_stageEnergy & GFC_STAGE_M_ENERGY)/4.0);
  }
  inline int FilterStatus::getStageEnergy() const{
    return m_stageEnergy;
  }
  inline int FilterStatus::getTcids()const{
    return m_tcids;
  }
  inline int FilterStatus::getEbfSize()const{
    return m_ebfsize;
  }
  inline const int * FilterStatus::getLayers()const {
    return m_layers;
  }

  inline std::vector<track> FilterStatus::getTracks()const{
    return m_tracks;
  }

  inline double FilterStatus::getSeparation() const{
    return m_separation;
  }

  inline bool FilterStatus::tracksExist() const{
    if(m_tracks.size()!=0)
      return true;
    return false;
  }

  inline const int* FilterStatus::getXcapture() const{
    return m_xcapture;
  }

  inline const int* FilterStatus::getYcapture() const{
    return m_ycapture;
  }
   
     inline const int FilterStatus::getNumLogsHit() const{
         return m_numLogsHit;
           }
     inline const LogInfo* FilterStatus::getLogInfo() const{
         return m_logData;
           }
           
  inline const float* FilterStatus::getLayerEnergy() const{
    return m_layerEnergy;
  }

  inline const int* FilterStatus::getXY00() const{
    return m_xy00;
  }

  inline const int* FilterStatus::getXY11() const{
    return m_xy11;
  }

  inline const int* FilterStatus::getXY22() const{
    return m_xy22;
  }

  inline const int* FilterStatus::getXY33() const{
    return m_xy33;
  }

  inline int FilterStatus::getTmsk() const{
    return m_tmsk;
  }

  inline int FilterStatus::getVetoWord() const{
    return m_vetoword;
  }

  inline void FilterStatus::setVetoWord(const int vetoword){
    m_vetoword=vetoword;
  }

  inline void FilterStatus::set(const unsigned int code){
    m_status=code;
  }

  inline void FilterStatus::setStageEnergy(const int stageEnergy){
    m_stageEnergy=stageEnergy;//must divide by 4 to get MeV units
  }

  inline void FilterStatus::setEbfSize(const int ebfsize){
    m_ebfsize = ebfsize;
  }

  inline void FilterStatus::setTcids(const int ids){
    m_tcids=ids;
  }


  inline void FilterStatus::setGemThrTkr(const int val){
    m_gem_thrTkr = val;
  }
   
   inline void FilterStatus::setGemCalHiLo(const int val){
    m_gem_calHiLo = val;
  } 
   inline void FilterStatus::setGemCondsumCno(const int val){
    m_gem_condsumCno = val;
  }
   inline void FilterStatus::setGemAcd_vetoes_XZ(const int val){
    m_gem_acd_vetoes_XZ = val;
  }
   inline void FilterStatus::setGemAcd_vetoes_YZ(const int val){
    m_gem_acd_vetoes_YZ = val;
  }
   inline void FilterStatus::setGemAcd_vetoes_XY(const int val){
    m_gem_acd_vetoes_XY = val;
  }
   inline void FilterStatus::setGemAcd_vetoes_RU(const int val){
    m_gem_acd_vetoes_RU = val;
  }
   inline void FilterStatus::setGemLivetime(const int val){
    m_gem_livetime = val;
  }
   inline void FilterStatus::setGemTrgtime(const int val){
    m_gem_trgtime = val;
  }  
   inline void FilterStatus::setGemPpstime(const int val){
    m_gem_ppstime = val;
  }
   inline void FilterStatus::setGemDiscarded(const int val){
    m_gem_discarded = val;
  } 
   inline void FilterStatus::setGemPrescaled(const int val){
    m_gem_prescaled = val;
  } 
   inline void FilterStatus::setGemSent(const int val){
    m_gem_sent = val;
  }


  inline void FilterStatus::setAcdMap(const int xz, const int yz, const int xy){
    m_acd_xz=xz;
    m_acd_yz=yz;
    m_acd_xy=xy;
  }

  inline void FilterStatus::setAcdStatus(const int tower, const int status){
    if(tower<16)
      m_acdStatus[tower]=status;
  }

  inline void FilterStatus::setLayers(const int *layerCode){
    for(int counter=0;counter<16;counter++)
      m_layers[counter]=layerCode[counter];
  }

  inline void FilterStatus::setProjections(const TFC_prjs &prjs){
    memcpy(&m_prjs, &prjs,sizeof(prjs));
  }

  inline void FilterStatus::setTkr(const EDR_tkr &tkr){
//    memcpy(&m_tkr, &tkr,sizeof(tkr));
    memcpy(m_tkr, &tkr,sizeof(tkr));
  }

  inline void FilterStatus::setSeparation(const double sep){
    m_separation=sep;
  }

  inline void FilterStatus::setTrack(const track &newTrack){
    m_tracks.push_back(newTrack);
  }

  inline void FilterStatus::setLayerEnergy(const int energy[8]){
    for(int counter=0;counter<8;counter++)
      m_layerEnergy[counter]=((float)energy[counter])/4.;//must divide by 4 to get MeV units
  }
   
   inline void FilterStatus::setLogData(int num, LogInfo tempLogData[]){
      m_numLogsHit = num;
      for (int i=0; i<num; i++) {
         m_logData[i] = tempLogData[i];
      }
   }
                   
  inline void FilterStatus::setCapture(const int xcapture[16], const int ycapture[16]){
    for(int counter=0;counter<16;counter++){
      m_xcapture[counter]=xcapture[counter];
      m_ycapture[counter]=ycapture[counter];
    }
  }

  inline void FilterStatus::setXY(const int xy00[16], const int xy11[16], const int xy22[16], const int xy33[16]){
    for(int counter=0;counter<16;counter++){
      m_xy00[counter]=xy00[counter];
      m_xy11[counter]=xy11[counter];
      m_xy22[counter]=xy22[counter];
      m_xy33[counter]=xy33[counter];
    }
  }

  inline void FilterStatus::setTmsk(int tmsk){
    m_tmsk=tmsk;
  }
  //****TEMP
  inline void FilterStatus::setHoughSep(const double houghsep){
      m_houghsep=houghsep;
  }
  inline double FilterStatus::getHoughSep() const{
      return m_houghsep;
  }

  inline void FilterStatus::setHighLayer(const double highestlayer){
      m_highestlayer=highestlayer;
  }
  inline double FilterStatus::getHighLayer() const{
      return m_highestlayer;
  }

  inline void FilterStatus::setSeparation2(const double sep2){
    m_separation2=sep2;
  }
  inline double FilterStatus::getSeparation2() const{
    return m_separation2;
  }
  inline void FilterStatus::setXlongest(const double x_report){
    m_x_report=x_report;
  }
  inline void FilterStatus::setYlongest(const double y_report){
    m_y_report=y_report;
  }
  inline void FilterStatus::setXavg(const double xavg){
    m_xavg=xavg;
  }
  inline void FilterStatus::setYavg(const double yavg){
    m_yavg=m_yavg;
  }
  inline void FilterStatus::setXfirst(const double long_firstanglex){
    m_long_firstanglex=long_firstanglex;
  }
  inline void FilterStatus::setYfirst(const double long_firstangley){
    m_long_firstangley=long_firstangley;
  }

  inline void FilterStatus::setXslopeL(const double xslopeL){
    m_xslopeL=xslopeL;
  }
  inline void FilterStatus::setYslopeL(const double yslopeL){
    m_yslopeL=yslopeL;
  }
  inline void FilterStatus::setXslopeAvg(const double xslopeAvg){
    m_xslopeAvg=xslopeAvg;
  }
  inline void FilterStatus::setYslopeAvg(const double yslopeAvg){
    m_yslopeAvg=yslopeAvg;
  }
  inline void FilterStatus::setXtower(const double xslopetower){
    m_xslopetower=xslopetower;
  }
  inline void FilterStatus::setYtower(const double yslopetower){
    m_yslopetower=yslopetower;
  }
  inline void FilterStatus::setXangleL(const double xangleL){
    m_xangleL=xangleL;
  }
  inline void FilterStatus::setYangleL(const double yangleL){
    m_yangleL=yangleL;
  }
  inline void FilterStatus::setXlongestB(const double xlongestB){
    m_xlongestB=xlongestB;
  }
  inline void FilterStatus::setYlongestB(const double ylongestB){
    m_ylongestB=ylongestB;
  }

  inline double FilterStatus::getXlongest() const{
    return m_x_report;
  }
  inline double FilterStatus::getYlongest() const{
    return m_y_report;
  }
  inline double FilterStatus::getXavg() const{
    return m_xavg;
  }
  inline double FilterStatus::getYavg() const{
    return m_yavg;
  }

  inline double FilterStatus::getXslopeL() const{
    return m_xslopeL;
  }
  inline double FilterStatus::getYslopeL() const{
    return m_yslopeL;
  }
  inline double FilterStatus::getXslopeAvg() const{
    return m_xslopeAvg;
  }
  inline double FilterStatus::getYslopeAvg() const{
    return m_yslopeAvg;
  }
  inline double FilterStatus::getXtower() const{
    return m_xslopetower;
  }
  inline double FilterStatus::getYtower() const{
    return m_yslopetower;
  }

  inline double FilterStatus::getXfirst() const{
    return m_long_firstanglex;
  }
  inline double FilterStatus::getYfirst() const{
    return m_long_firstangley;
  }
  inline double FilterStatus::getXangleL() const{
    return m_xangleL;
  }
  inline double FilterStatus::getYangleL() const{
    return m_yangleL;
  }
  inline double FilterStatus::getXlongestB() const{
    return m_xlongestB;
  }
  inline double FilterStatus::getYlongestB() const{
    return m_ylongestB;
  }
  //****TEMP

  inline std::ostream& FilterStatus::fillStream(std::ostream &s) const{
    s<<"Filter Return Code: "<<m_status<<std::endl;
    s<<"Filter code for StageEnergy in CAL: "<<m_stageEnergy<<std::endl;
    s << "VetoWord: " << getVetoWord() << std::endl;
    s << "Acd Status: " << std::endl;
    unsigned int i;
    for (i=0; i<16;i++) s << m_acdStatus[i] << " ";
    s << std::endl;
    return s;
  }


  inline std::ostream& operator<<(std::ostream &s, const FilterStatus &obj){
    return obj.fillStream(s);
  }

}//namespace OnboradFilterTds


#endif
