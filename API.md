The selected approach to the API architecture is REST. Actually not all the features of REST approach would be realised. Such constraits as self-descriptive messages and HATEOAS would be 
be sacrificed.

### REPRESENTATIONS

***User representation***
```html
<user>
	<id></id>
	<cash></cash>
</user>
```

***Bet representation***
```html
<bet>
	<id></id>
	<event_id></event_id>
	<user_id></user_id>
	<sum></sum>
	<side></side>
</bet>
```

***Event short representation***
```html
<event>
	<name></name>
	<id></id>
</event>
```

***Event full representation***
```html
<event>
	<name></name>
	<id></id>
	<description></description>
	<date></date>
	<sum_side_1></sum_side_1>
	<sum_side_2></sum_side_2>
	<bets>
		<bet>
			<id></id>
		</bet>
		...
		<bet>
			<id></id>
		</bet>
	</bets>
</event>
```

***Events collection representation***
```html
<events>
	<event short representation>
	...
	<event short representation>
</events>
```

### REQUESTS

***Get a list of all events***

* GET /events

* Response: 	

	_Events_ _collection_ _representation_

***Create a new event***

* POST /events

* Request:
```html
	<event>
		<name></name>
		<description></description>
		<date></date>
	</event>
```

* Response: 	
		_201_ _Created_
		and
		_Location:_ _/events/event_id_
		and
		_Event_ _full_ _representation_

***Get full event info***

* GET /events/event_id

* Response:

	_404_ _Not_ _found_ _or_ _event_ _full_ _representation_

***Bet on an event***

* POST /bets

* Request:	
```html
	<bet>
		<event_id></event_id>
		<user_id></user_id>
		<sum></sum>
		<side></side>
	</bet>
```

* Response:	

	_404_ _Not_ _found_ _or_ _202_ _Accepted_
	
	_Location:_ _/bets/bet_id_
	
	_Bet_ _representation_

***Delete an event***

* DELETE /events/event_id

* Response:

	_404_ _Not_ _found_ _or_ _event_ _full_ _representation_
