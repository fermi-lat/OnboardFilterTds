#ifndef Obf_TFC_prjs_h
#define Obf_TFC_prjs_h

/** @file Obf_TFC_prjs.h
  * @author Tracy Usher
  * 
  * File defining output to TDS of OnboardFilter reconstruction objects
  *
  * $Header: /nfs/slac/g/glast/ground/cvs/OnboardFilterTds/OnboardFilterTds/Obf_TFC_prjs.h,v 1.2 2008/05/17 02:31:21 usher Exp $
**/

// Include the FSW header file defining the tracking structures
#include "OnboardFilterTds/FSWHeaders/TFC_prjDef.h"

namespace OnboardFilterTds {

class Obf_TFC_prjs
{
public:
    Obf_TFC_prjs() : m_prjs(0) {}
    Obf_TFC_prjs(const TFC_prjs* prjs);

    ~Obf_TFC_prjs();

    // Initialize an existing object
    void initialize(const TFC_prjs* prjs);

    // Recover information
    unsigned short int getMaxCnt()                        const {return m_maxCnt;}
    unsigned short int getCurCnt()                        const {return m_curCnt;}
    unsigned short int getTwrMask()                       const {return m_twrMsk;}
    const TFC_prjDir&  getPrjDir(int towerId)             const {return m_dir[towerId];}
    const TFC_prjList& getTopDir(int prjIdx, int towerId) const {return m_top[prjIdx][towerId];}
    const TFC_prjList& getBotDir(int prjIdx, int towerId) const {return m_top[prjIdx][towerId];}
    const TFC_prj&     getPrj(int idx)                    const {return m_prjs[idx];}

private:

    unsigned short int m_maxCnt;     /*!< Maximum # of projections available  */
    unsigned short int m_curCnt;     /*!< Current # of projections in use     */
    unsigned short int m_twrMsk;     /*!< Mask of the tower with projections  */
    TFC_prjDir         m_dir[16];    /*!< Directory of the projections by twr */
    TFC_prjList        m_top[2][16]; /*!< List of projections sorted by
                                          their top most tower                */
    TFC_prjList        m_bot[2][16]; /*!< List of projections sorted by
                                          their bottom most tower             */
    TFC_prj*           m_prjs;       /*!< List of projections                 */
};

} // end of namespace

#endif
