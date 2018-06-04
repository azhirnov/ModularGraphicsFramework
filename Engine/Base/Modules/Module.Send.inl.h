// This is part of 'Module::Send' function.
{
	using FixedMapRange_t	= MixedSizeArray< MessageHandler::HandlersMap_t::CPair_t, 32 >;

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
		
	for (auto& handler : temp)
	{
		handler.second.func( handler.second.ptr, handler.second.data, var_msg );
	}
	return not temp.Empty();
}
