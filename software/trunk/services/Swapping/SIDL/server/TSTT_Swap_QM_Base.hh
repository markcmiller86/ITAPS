#ifndef included_TSTTM_Swap_QM_Base_hh
#define included_TSTTM_Swap_QM_Base_hh

namespace TSTT_Swap {
  class QM_Base {
  public:
    QM_Base() {}
    virtual ~QM_Base() {}
    virtual bool shouldMinimize() throw(::TSTTB::Error) = 0;
    virtual double calcQuality(::sidl::array<double> coords,
			       int32_t coords_size) throw(::TSTTB::Error) = 0;
  };
}

#endif
