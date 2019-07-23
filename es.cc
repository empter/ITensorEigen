#include "itensor/all.h"
#include <stdio.h>
#include <Eigen/Dense>

namespace Eigen {
  template<typename V>
  void esystem(int N, V* Hdata, V* Udata, V* Ddata)
  {
    Map<Matrix<V,Dynamic,Dynamic>> hmat(Hdata, N, N);
    Map<Matrix<V,Dynamic,Dynamic>> umat(Udata, N, N);
    Map<Matrix<V,Dynamic,1>> eig(Ddata, N);
    ComplexEigenSolver<Matrix<V,Dynamic,Dynamic>> ces;
    ces.compute(hmat);
    umat = ces.eigenvectors();
    eig = ces.eigenvalues();
  }
}

using namespace itensor;

auto extractCplx = [](Dense<Cplx> const& d)
    {
    return d.store;
    };
auto extractReal = [](Dense<Real> const& d)
    {
    return d.store;
    };

int
main(int argc, char* argv[] )
    {
      if(argc != 2)
        {
          printfln("Usage: %s N",argv[0]);
          return 0;
        }
        auto N = std::atoi(argv[1]);
        auto i = Index(N);
        auto H = randomITensorC(i,prime(i));
        std::vector<Cplx> u,d;
        u.resize(N*N);
        d.resize(N);
        auto h = applyFunc(extractCplx,H.store());//extract data in itensor H
        Eigen::esystem(N,h.data(),u.data(),d.data());

        auto store = DenseCplx(u.size());
        auto uc = u.begin();
        for(decltype(u.size()) n = 0; n < u.size(); ++uc, ++n)
            {
            store[n] = *uc;
            }
        auto U = ITensor({i,prime(i)},move(store));
        // check the first eigenvalue, U1 is the first eigenvector
        auto U1 = U * setElt(prime(i)=1);
        PrintData(norm(H*prime(U1)-d[0]*U1));// should be close to zero
        return 0;
    }
    
