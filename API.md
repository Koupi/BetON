The selected approach to the API architecture is REST. Actually not all the features of REST approach would be realised. Such constraits as self-descriptive messages and HATEOAS would be 
be sacrificed.

### REPRESENTATIONS

1. User representation
```html
<user>
	<id></id>
</user>
```

2. Event short representation
```html
<event>
	<name></name>
	<id></id>
</event>
```

3. Event full representation
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

4. Events collection representation
```html
<events>
	<event short representation>
	...
	<event short representation>
</events>
```

### REQUESTS
1. Get a list of all events
GET /events
Response: 	events collection representation

2. Create a new event
POST /events
Request:
```html
	<event>
		<name></name>
		<description></description>
		<date></date>
	</event>
```
Response: 	201 Created
		Location: /events/event_id
  		event full representation

3. Get full event info
GET /events/event_id
Response:	404 Not found or event full representation

4. Bet on an event
PUT /events/event_id/bets
Request:	
```html
	<user>
		<bet></bet>
	</user>
```
Response:	404 Not found or event full representation

5. Delete an event
DELETE /events/event_id
Response:	404 Not found or event full representation
