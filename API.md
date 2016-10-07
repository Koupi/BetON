The selected approach to the API architecture is REST. Actually not all the features of REST approach would be realised. Such constraits as self-descriptive messages and HATEOAS would be 
be sacrificed.

### REPRESENTATIONS

***User representation***
```html
<user>
	<id></id>
</user>
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
	<coefficient></coefficient>
	<bets>
		<user>
			<id></id>
			<bet></bet>
		</user>
		...
		<user>
			<id></id>
			<bet></bet>
		</user>
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

	_events_ _collection_ _representation_

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
		
		_Location:_ _/events/event_id_
		
  		_event_ _full_ _representation_

***Get full event info***

* GET /events/event_id

* Response:

	_404_ _Not_ _found_ _or_ _event_ _full_ _representation_

***Bet on an event***

* PUT /events/event_id/bets

* Request:	
```html
	<user>
		<bet></bet>
	</user>
```

* Response:	

	_404_ _Not_ _found_ _or_ _event_ _full_ _representation_

***Delete an event***

* DELETE /events/event_id

* Response:

	_404_ _Not_ _found_ _or_ _event_ _full_ _representation_
