#ifndef __COINPAINTER_HPP__
#define __COINPAINTER_HPP__

#include <NDK/Entity.hpp>

#include <unordered_map>

namespace SMB
{
	class Coin;
	class StateContext;
}

namespace SMB
{
	class CoinPainter
	{
		public:

			CoinPainter(StateContext& context);

			void Clear();
			bool CreateCoin(const Coin& coin);

			void Update(const Coin& coin, float elapsedTime);

		private:

			Ndk::EntityHandle& GetEntityAssociatedWith(const Coin& coin);

			std::unordered_map<int, Ndk::EntityHandle> m_coinEntityMap;
			StateContext& m_context;
	};
}

#endif // __COINPAINTER_HPP__
