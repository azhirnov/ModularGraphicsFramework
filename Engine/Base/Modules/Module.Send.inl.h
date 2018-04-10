// This is part of 'Module::Send' function.
{
	using FixedMapRange_t	= MixedSizeArray< MessageHandler::HandlersMap_t::const_pair_t, 32 >;

	FixedMapRange_t	temp;
	{
		auto&	handlers = _msgHandler._handlers;

		usize	first;
		if ( handlers.FindFirstIndex( var_msg.GetValueTypeId(), OUT first ) )
		{
			for (usize i = first; i < handlers.Count() and handlers[i].first == var_msg.GetValueTypeId(); ++i)
			{
				if ( handlers[i].second.ptr.Lock() == null )
				{
					handlers.EraseByIndex( i );
					--i;
					continue;
				}

				temp.PushBack( handlers[i] );
			}
		}
	}
		
	FOR( i, temp )
	{
		auto&	handler = temp[i].second;

		handler.func( handler.ptr, handler.data, var_msg );
	}
	return not temp.Empty();
}
