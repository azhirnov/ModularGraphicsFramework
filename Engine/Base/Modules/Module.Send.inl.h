// This is part of 'Module::Send' function.
{
	using FixedMapRange_t	= MixedSizeArray< MessageHandler::Handler, 32 >;
	using HandlerSearch		= MessageHandler::HandlerSearch;

	FixedMapRange_t	temp;
	{
		auto&	handlers = _msgHandler._handlers;

		usize	first;
		if ( handlers.CustomSearch().FindFirstIndex( HandlerSearch{ var_msg.GetValueTypeId() }, OUT first ) )
		{
			for (usize i = first; i < handlers.Count() and handlers[i].first.id == var_msg.GetValueTypeId(); ++i)
			{
				/*if ( handlers[i].second.ptr.Lock() == null )
				{
					handlers.EraseByIndex( i );
					--i;
					continue;
				}*/

				temp.PushBack( handlers[i].second );
			}
		}
	}
		
	for (auto& handler : temp)
	{
		handler.func( handler.ptr, handler.data, var_msg );
	}
	return not temp.Empty();
}
