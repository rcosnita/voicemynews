# Summary

In this document we describe the analytics layer from voice my news. The intention is to obtain insights from every single
instance of our application. Based on this insights we will be able to take decisions and improve the app experience.

Our analytics layer is event driven, meaning it will accept events emitted from various components of voice my news.

Initially, these events will be stored in google analytics using
[GA custom events](https://developers.google.com/analytics/devguides/collection/analyticsjs/events).

In the future, we can easily change the events backend to something else.

# Functional requirements

* Native UI of the app must be able to log analytics events.
* Native common code of the app must be able to log analytics events.
* JS common code of the app must be able to log analytics events.

# Technical summary

![Analytics layer](images/analytics-layer.png?raw=true)

In order to be able to integrate analytics solution into the app for all target platforms we take the following approach:

1. Each platform will implement a specific contract for providing access to a web browser.
1. We will implement an analytics web page which is loaded by the web browser when the app starts.
1. Each layer of the application will be able to send custom events to the analytics web page using platform specific implementation
(post messages should work on all available platforms).

## Events model

An analytic event from voicemynews has the following attributes:

| **Attribute name** | **Value type** | **Required** | **Description** |
| ------------------ | -------------- | ------------ | --------------- |
| **eventCategory**  | text | yes | Typically the object that was interacted with (e.g. 'Video'). |
| **eventAction**    | text | yes | The type of interaction (e.g. 'play'). |
| **eventLabel**     | text | yes | Useful for categorizing events (e.g. 'Fall Campaign'). |
| **eventValue**     | integer | yes | A numeric value associated with the event (e.g. 42). |

# Revisions history

| **Version** | **Date** | **Author** | **Description** |
|-------------|----------|------------|-----------------|
| 1.0 | 27.12.2016 | Radu Viorel Cosnita | This revision contains the initial technical specification for analytics layer. |