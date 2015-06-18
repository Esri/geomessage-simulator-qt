# GeoMessage Specification

Esri created the GeoMessage specification as a pattern for messages that are sent and received by applications and components that are part of the [ArcGIS for the Military](http://solutions.arcgis.com/military) solution. This document explains the GeoMessage format for those who want to generate, process, or otherwise use GeoMessages.

## Sections

* [XML Schema for GeoMessages](#xml-schema-for-geomessages)
* [Description of the GeoMessage Format](#description-of-the-geomessage-format)
* [Sample GeoMessages](#sample-geomessages)

## XML Schema for GeoMessages

GeoMessages are expressed in XML. A formal schema for each GeoMessage type is defined in the [schema directory](./schema/), using XML Schema Definition (XSD) files. That directory also contains a generic schema against which any valid GeoMessage will validate.

## Description of the GeoMessage Format

A `<geomessages>` element contains one or more `<geomessage>` elements. Each GeoMessage represents a message that a military user or system might send, like a spot report or a track report. Each `<geomessage>` element may have an attribute `v` specifying the GeoMessage version in use for that GeoMessage. If `v` is not specified, implementers could try parsers for various GeoMessage versions if available. *(Note: at the initial 1.1 release of this specification, only `v="1.1"` should be used. In some apps and samples, `v="1.0"` has been used but was never formally defined, so a GeoMessage 1.1 parser should accept both `v="1.1"` and `v="1.0"`.)*

Most `<geomessage>` elements contain at least the following elements:

- `<_type>`: the element value specifies the type of GeoMessage. Valid values:
    - `bedavail`: a Bed Availability and Element Status (BEDAVAIL).
    - `chemlight`: a digital chemlight.
    - `eod`: an explosive ordnance disposal (EOD) 9-line report.
    - `gsrrep`
    - `medevac`: a Medical Evacuation Request (MEDEVAC).
    - `position_report`: a Track/Point Report (TRKREP).
    - `sensor_obs`
    - `sitrep`: a Commander's Situation Report (SITREP).
    - `slantrep`: a Slant Report (SLANTREP).
    - `spotrep`: a Spot Report (SPOTREP).
- `<_action>`: the element value specifies whether the message is being created, updated, highlighted, un-highlighted, or removed. Valid values:
    - `remove`: remove the GeoMessage with the corresponding `_id`.
    - `removeall`: remove all GeoMessages of the corresponding `_type`.
    - `select`: highlight the GeoMessage with the corresponding `_id`. This is often used to indicate emergency or 911 status.
    - `un-select`: stop highlighting the GeoMessage with the corresponding `_id`.
    - `update`: update the GeoMessage with the corresponding `_id`, or, if no previous GeoMessage has the same `_id`, create a new GeoMessage.
- `<_id>`: a unique identifier for the GeoMessage. This ID does not correspond to a unit or equipment but is a unique ID for the GeoMessage. To update, select, un-select, or remove a previous GeoMessage, send a new GeoMessage with the same `_id` and the appropriate `_action`.
- `<_wkid>`: the [Well-Known ID](http://wiki.gis.com/wiki/index.php/Well-Known_ID) of the spatial reference in which this GeoMessage's `_control_points` are expressed. If absent, WGS 1984 (WKID 4326), a geographic coordinate system (i.e. longitude and latitude), is assumed.
- `<sic>`: a MIL-STD-2525C or APP-6B symbol ID code (SIC or SIDC) for the subject of the GeoMessage. The subject of the GeoMessage may be the sender, as in a Track/Point Report, which is about the sending unit or equipment, or it may be a different entity, as in a Spot Report, which is normally about an entity other than the sender.
- `<_control_points>`: a semicolon-delimited list of 2D points, where each point is expressed as `x,y`. A list of one point denotes a point geometry. A list of two points denotes a line (a.k.a. polyline) geometry. A list of three or more points denotes either a line geometry or an area (a.k.a. polygon) geometry.
- `<datetimesubmitted>`: the GeoMessage's timestamp, in the following format: `2015-07-26 00:00:00`. In a future GeoMessage version, XML `dateTime` will be used.
- `<uniquedesignation>`: a unique and often human-readable unique identifier for the subject of the GeoMessage. See `<sic>` above for a discussion of the subject of a GeoMessage.

A GeoMessage may contain many other elements, depending on `<_type>`, and these elements are specified in the XSD files in the [schema directory](./schema/).

## Sample GeoMessages

