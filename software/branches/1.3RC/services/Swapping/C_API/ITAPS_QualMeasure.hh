#ifndef included_ITAPS_QualMeasure_hh
#define included_ITAPS_QualMeasure_hh

#include "iBase.h"

#include "ITAPS_Swap_config.hh"

#ifdef HAVE_MESQUITE_HPP
// Mesquite includes
#include "ElementQM.hpp"
#include "Mesquite.hpp"
#include "PatchData.hpp"
#endif

namespace ITAPS_Swap {
  /*! \brief A virtual base class providing a common interface to all
   * quality measures.
   *
   * Important caveat: quality measures must be cell-based.  That is,
   * the calculation must be based solely on the coordinates of the
   * vertices of a tetrahedron.  While there are other useful quality
   * measures, this restricted form is what the swapping drivers
   * expect. 
   */  
  class QM_Base {
  public:
    QM_Base() {}
    virtual ~QM_Base() {}
    /*! \brief Returns true iff the quality measure should be minimized by
     * swapping. */
    virtual bool shouldMinimize() const
      throw(iBase_ErrorType) = 0;
    /*! \brief Evaluates the quality of a single (actual or possible) cell.
     *
     * Because the vertices are specified only by their coordinates,
     * both current cells and post-swapping candidates can easily be
     * accomodated.
     *
     * \param[in] coords Coordinates of vertices (stored as x1,y1,z1,
     * x2,y2,z2, etc; in 2D no z-coordinates are passed).
     *
     * \param[in] coords_size Number of coordinates passed in.
     *
     * \return Quality measure for these vertices.
     */
    virtual double calcQuality(const double coords[],
			       const int coords_size) const
      throw(iBase_ErrorType) = 0;
  };

  class Delaunay3D : public QM_Base {
  public:
    Delaunay3D() {}
    virtual ~Delaunay3D() {}
    virtual bool shouldMinimize() const
      throw(iBase_ErrorType) {return false;}
    /*! This quality measure actually computes the ratio of shortest
     * edge length to circumradius, since Delaunay isn't a single cell
     * quality measure.  This doesn't affect the swapping code directly,
     * since there's a hard-coded swapping decision for Delaunay.
     */
    virtual double calcQuality(const double coords[],
			       const int coords_size) const
      throw(iBase_ErrorType);
  };
    
  class MaxMinSine3D : public QM_Base {
  public:
    MaxMinSine3D() {}
    virtual ~MaxMinSine3D() {}
    virtual bool shouldMinimize() const
      throw(iBase_ErrorType) {return false;}
    virtual double calcQuality(const double coords[],
			       const int coords_size) const
      throw(iBase_ErrorType);
  };
    
  class MinMaxAngle3D : public QM_Base {
  public:
    MinMaxAngle3D() {}
    virtual ~MinMaxAngle3D() {}
    virtual bool shouldMinimize() const
      
      throw(iBase_ErrorType) {return true;}
    virtual double calcQuality(const double coords[],
			       const int coords_size) const
      throw(iBase_ErrorType);
  };
    
#ifdef HAVE_MESQUITE_HPP
  class Mesquite3D : public QM_Base {
    Mesquite::ElementQM *m_mesqQM;
  public:
    Mesquite3D(Mesquite::ElementQM *mesqQM) :
      QM_Base(), m_mesqQM(mesqQM) {}
    virtual ~Mesquite3D() {}
    virtual bool shouldMinimize() const
      throw(iBase_ErrorType) {return (m_mesqQM->get_negate_flag() == 1);}
    /*! This quality measure actually computes the ratio of shortest
     * edge length to circumradius, since Delaunay isn't a single cell
     * quality measure.  This doesn't affect the swapping code directly,
     * since there's a hard-coded swapping decision for Delaunay.
     */
    virtual double calcQuality(const double coords[],
			       const int coords_size) const
      throw(iBase_ErrorType)
    {
      Mesquite::PatchData PD;
      size_t conn[] = {0,1,2,3};
      Mesquite::EntityTopology topo = Mesquite::TETRAHEDRON;
      Mesquite::MsqError err;
      PD.fill(4, coords, 1, topo, conn, NULL, err);
      MSQ_CHKERR(err);
      double value;
      m_mesqQM->evaluate(PD, 0, value, err);
      MSQ_CHKERR(err);
      return value;
    }
  };
#endif
}

#endif
