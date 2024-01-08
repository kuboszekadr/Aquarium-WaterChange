# Notification

Class similar to `Logger`, but much simpler, since only two parts of the message are important:
- `title` - title of the notification,
- `message` - body.

Since it is independent from `Logger` user must define separete stream (for instance endpoint).