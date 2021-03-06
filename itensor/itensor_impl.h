//
// Distributed under the ITensor Library License, Version 1.2
//    (See accompanying LICENSE file.)
//
#ifndef __ITENSOR_ITENSOR_IMPL_H_
#define __ITENSOR_ITENSOR_IMPL_H_

//
// Template Method Implementations
//

namespace itensor {

//template<>
//template <typename... IVals>
//ITensor::
//ITensorT(const IndexVal& iv1, 
//         const IVals&... rest)
//  : scale_(1.)
//    {
//    const size_t size = 1+sizeof...(rest);
//    auto ivs = std::array<IndexVal,size>{{iv1,rest...}};
//    std::array<Index,size> inds;
//    for(size_t j = 0; j < size; ++j) inds[j] = ivs[j].index;
//    is_ = IndexSet(inds);
//    store_ = newITData<DenseReal>(area(is_),0.);
//    set(iv1,rest...,1.);
//    }

// TODO: implement special code for doing this operation
//       more efficiently
inline ITensor& 
operator*=(ITensor & T, IndexVal const& iv) { return T *= setElt(iv); } 
ITensor inline
operator*(IndexVal const& iv, ITensor const& B) 
    { 
    auto A = setElt(iv);
    A *= B; 
    return A; 
    }

ITensor inline
operator*(IndexVal const& iv1, IndexVal const& iv2) 
    { 
    auto t = setElt(iv1);
    return (t *= iv2); 
    }

ITensor inline
operator*(IndexVal const& iv1, Cplx val) 
    { 
    auto res = setElt(iv1);
    res *= val; 
    return res; 
    }

ITensor inline
delta(IndexSet const& is)
    { 
    auto len = minM(is);
    return ITensor(std::move(is),DiagReal(len,1.));
    }

template<typename... Inds>
ITensor
delta(Index const& i1,
      Inds const&... inds)
    { 
    return delta(IndexSet(i1,inds...));
    }

ITensor inline
delta(std::vector<Index> const& is)
    {
    return delta(IndexSet(is));
    }

template<size_t N>
ITensor
delta(std::array<Index,N> const& is)
    {
    return delta(IndexSet(is));
    }


ITensor inline
delta(std::initializer_list<Index> is)
    {
    return delta(IndexSet(is));
    }

template<typename Container, class>
ITensor
diagTensor(Container const& C, 
           IndexSet const& is)
    { 
#ifdef DEBUG
    using size_type = decltype(C.size());
    //Compute min of all index dimensions
    auto minm = is[0].m();
    for(const auto& ind : is)
        if(ind.m() < minm) minm = ind.m();
    if(C.size() != size_type(minm))
        {
        println("minm = ",minm);
        println("C.size() = ",C.size());
        Error("Wrong size of data in diagonal ITensor constructor");
        }
#endif
    using value_type = typename Container::value_type;
    return ITensor(std::move(is),Diag<value_type>(C.begin(),C.end()));
    }


template<typename Container, typename... Inds>
ITensor
diagTensor(Container const& C, 
           Index const& i1,
           Inds &&... inds)
    { 
    return diagTensor(C,IndexSet(i1,std::forward<Inds>(inds)...));
    }

template<typename Container>
ITensor
diagTensor(Container const& C, 
           std::vector<Index> const& is)
    { 
    return diagTensor(C,IndexSet(is));
    }

template<typename Container,
         size_t N>
ITensor
diagTensor(Container const& C, 
           std::array<Index,N> const& is)
    { 
    return diagTensor(C,IndexSet(is));
    }

template<typename Container>
ITensor
diagTensor(Container const& C,
           std::initializer_list<Index> is)
    { 
    return diagTensor(C,IndexSet(is));
    }

template<typename IndexT>
ITensorT<IndexT>
randomTensor(const IndexSetT<IndexT>& inds)
    {
    return random(ITensorT<IndexT>{inds});
    }

} //namespace itensor

#endif
