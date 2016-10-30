<h1>CAP</h1>

<h3>Description</h3>

The developed system offers users to make a bet on some event. Users can see the approximate sum of all the bets on both sides. The sum is approximate because of its possibility to be changed while being displayed for the concrete user when other users continue making bets. It's important for the system not to loose any bet and allow all users to participate. Most users are likely to bet on during last hour before closing of an event.

<h3>Losses</h3>
A sacrifice of the consistency when an event is about to begin is thought to be justified. So users can see not the latest sum of bets, especially when the amount of bets is high enough. But that's not a problem because the system will consider all the bets after closing an event and charge money with a final coefficient.

<h3>Guarantees</h3>
The system guarantees safety of all the bets and a fair final money charging. 

<h3>Technical means</h3>
Technically the ideas above would be represented as a CP-system for general bets and an AP-system working during last hour before an event.

The CP-system would be provided by MongoDB. As the AP-system realization each server will have a local database or just an array with an information (that would be decided a little bit later).

One more important point is caching. The system will cache the information about some most popular events so that not to spend a lot of time while waiting for a respond from main MongoDB.
