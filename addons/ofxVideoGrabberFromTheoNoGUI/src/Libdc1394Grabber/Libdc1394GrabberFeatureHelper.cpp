#include "Libdc1394GrabberFeatureHelper.h"


dc1394feature_t Libdc1394GrabberFeatureHelper::AVidFeatureToLibdcFeature( int _avidFeatureEnum )
{

	switch ( _avidFeatureEnum )
	{

		case FEATURE_BRIGHTNESS:
			return DC1394_FEATURE_BRIGHTNESS;
			break;

		case FEATURE_EXPOSURE:
			return DC1394_FEATURE_EXPOSURE;
			break;

		case FEATURE_SHARPNESS:
			return DC1394_FEATURE_SHARPNESS;
			break;

		case FEATURE_WHITE_BALANCE:
			return DC1394_FEATURE_WHITE_BALANCE;
			break;

		case FEATURE_HUE:
			return DC1394_FEATURE_HUE;
			break;

		case FEATURE_SATURATION:
			return DC1394_FEATURE_SATURATION;
			break;

		case FEATURE_GAMMA:
			return DC1394_FEATURE_GAMMA;
			break;

		case FEATURE_SHUTTER:
			return DC1394_FEATURE_SHUTTER;
			break;

		case FEATURE_GAIN:
			return DC1394_FEATURE_GAIN;
			break;

		case FEATURE_IRIS:
			return DC1394_FEATURE_IRIS;
			break;

		case FEATURE_FOCUS:
			return DC1394_FEATURE_FOCUS;
			break;

		case FEATURE_TEMPERATURE:
			return DC1394_FEATURE_TEMPERATURE;
			break;

		case FEATURE_TRIGGER:
			return DC1394_FEATURE_TRIGGER;
			break;

		case FEATURE_TRIGGER_DELAY:
			return DC1394_FEATURE_TRIGGER_DELAY;
			break;

		case FEATURE_WHITE_SHADING:
			return DC1394_FEATURE_WHITE_SHADING;
			break;

		case FEATURE_FRAME_RATE:
			return DC1394_FEATURE_FRAME_RATE;
			break;

		case FEATURE_ZOOM:
			return DC1394_FEATURE_ZOOM;
			break;

		case FEATURE_PAN:
			return DC1394_FEATURE_PAN;
			break;

		case FEATURE_TILT:
			return DC1394_FEATURE_TILT;
			break;

		case FEATURE_OPTICAL_FILTER:
			return DC1394_FEATURE_OPTICAL_FILTER;
			break;

		case FEATURE_CAPTURE_SIZE:
			return DC1394_FEATURE_CAPTURE_SIZE;
			break;

		case FEATURE_CAPTURE_QUALITY:
			return DC1394_FEATURE_CAPTURE_QUALITY;
			break;


		default:
			return DC1394_FEATURE_MIN;
			break;
	}
}

int Libdc1394GrabberFeatureHelper::libdcFeatureToAVidFeature( dc1394feature_t _libdcFeatureEnum )
{

	switch ( _libdcFeatureEnum )
	{

	case DC1394_FEATURE_BRIGHTNESS:
		return FEATURE_BRIGHTNESS;
		break;

	case DC1394_FEATURE_EXPOSURE:
		return FEATURE_EXPOSURE;
		break;

	case DC1394_FEATURE_SHARPNESS:
		return FEATURE_SHARPNESS;
		break;

	case DC1394_FEATURE_WHITE_BALANCE:
		return FEATURE_WHITE_BALANCE;
		break;

	case DC1394_FEATURE_HUE:
		return FEATURE_HUE;
		break;

	case DC1394_FEATURE_SATURATION:
		return FEATURE_SATURATION;
		break;

	case DC1394_FEATURE_GAMMA:
		return FEATURE_GAMMA;
		break;

	case DC1394_FEATURE_SHUTTER:
		return FEATURE_SHUTTER;
		break;

	case DC1394_FEATURE_GAIN:
		return FEATURE_GAIN;
		break;

	case DC1394_FEATURE_IRIS:
		return FEATURE_IRIS;
		break;

	case DC1394_FEATURE_FOCUS:
		return FEATURE_FOCUS;
		break;

	case DC1394_FEATURE_TEMPERATURE:
		return FEATURE_TEMPERATURE;
		break;

	case DC1394_FEATURE_TRIGGER:
		return FEATURE_TRIGGER;
		break;

	case DC1394_FEATURE_TRIGGER_DELAY:
		return FEATURE_TRIGGER_DELAY;
		break;

	case DC1394_FEATURE_WHITE_SHADING:
		return FEATURE_WHITE_SHADING;
		break;

	case DC1394_FEATURE_FRAME_RATE:
		return FEATURE_FRAME_RATE;
		break;

	case DC1394_FEATURE_ZOOM:
		return FEATURE_ZOOM;
		break;

	case DC1394_FEATURE_PAN:
		return FEATURE_PAN;
		break;

	case DC1394_FEATURE_TILT:
		return FEATURE_TILT;
		break;

	case DC1394_FEATURE_OPTICAL_FILTER:
		return FEATURE_OPTICAL_FILTER;
		break;

	case DC1394_FEATURE_CAPTURE_SIZE:
		return FEATURE_CAPTURE_SIZE;
		break;

	case DC1394_FEATURE_CAPTURE_QUALITY:
		return FEATURE_CAPTURE_QUALITY;
		break;


	default:
		return -1;
		break;
	}


}

int Libdc1394GrabberFeatureHelper::libdcStringToAVidFeature(string _featureString)
{
	if( _featureString == "BRIGHTNESS" )
	{
		return FEATURE_BRIGHTNESS;
	}
	else if ( _featureString == "EXPOSURE" )
	{
		return FEATURE_EXPOSURE;
	}
	else if ( _featureString == "SHARPNESS" )
	{
		return FEATURE_SHARPNESS;
	}	
	else if ( _featureString == "WHITE_BALANCE" )
	{
		return FEATURE_WHITE_BALANCE;
	}
	else if ( _featureString == "HUE" )
	{
		return FEATURE_HUE;
	}
	else if ( _featureString == "SATURATION" )
	{
		return FEATURE_SATURATION;
	}
	else if ( _featureString == "GAMMA" )
	{
		return FEATURE_GAMMA;
	}
	else if ( _featureString == "SHUTTER" )
	{
		return FEATURE_SHUTTER;
	}
	else if ( _featureString == "GAIN" )
	{
		return FEATURE_GAIN;
	}
	else if ( _featureString == "IRIS" )
	{
		return FEATURE_IRIS;
	}	
	else if ( _featureString == "FOCUS" )
	{
		return FEATURE_FOCUS;
	}
	else if ( _featureString == "TEMPERATURE" )
	{
		return FEATURE_TEMPERATURE;
	}
	else if ( _featureString == "TRIGGER" )
	{
		return FEATURE_TRIGGER;
	}
	else if ( _featureString == "TRIGGER_DELAY" )
	{
		return FEATURE_TRIGGER_DELAY;
	}
	else if ( _featureString == "WHITE_SHADING" )
	{
		return FEATURE_WHITE_SHADING;
	}
	else if ( _featureString == "FRAME_RATE" )
	{
		return FEATURE_FRAME_RATE;
	}
	else if ( _featureString == "ZOOM" )
	{
		return FEATURE_ZOOM;
	}
	else if ( _featureString == "PAN" )
	{
		return FEATURE_PAN;
	}
	else if ( _featureString == "TILT" )
	{
		return FEATURE_TILT;
	}
	else if ( _featureString == "OPTICAL_FILTER" )
	{
		return FEATURE_OPTICAL_FILTER;
	}	
	else if ( _featureString == "CAPTURE_SIZE" )
	{
		return FEATURE_CAPTURE_SIZE;
	}
	else if ( _featureString == "CAPTURE_QUALITY" )
	{
		return FEATURE_CAPTURE_QUALITY;
	}	
		// what should we return here?
	return -1;
	
}

/*
#define FEATURE_BRIGHTNESS			DC1394_FEATURE_BRIGHTNESS
#define FEATURE_EXPOSURE			DC1394_FEATURE_EXPOSURE
#define FEATURE_SHARPNESS			DC1394_FEATURE_SHARPNESS
#define FEATURE_WHITE_BALANCE		DC1394_FEATURE_WHITE_BALANCE
#define FEATURE_HUE					DC1394_FEATURE_HUE
#define FEATURE_SATURATION			DC1394_FEATURE_SATURATION
#define FEATURE_GAMMA				DC1394_FEATURE_GAMMA
#define FEATURE_SHUTTER				DC1394_FEATURE_SHUTTER
#define FEATURE_GAIN				DC1394_FEATURE_GAIN
#define FEATURE_IRIS				DC1394_FEATURE_IRIS
#define FEATURE_FOCUS				DC1394_FEATURE_FOCUS
#define FEATURE_TEMPERATURE			DC1394_FEATURE_TEMPERATURE
#define FEATURE_TRIGGER				DC1394_FEATURE_TRIGGER
#define FEATURE_TRIGGER_DELAY		DC1394_FEATURE_TRIGGER_DELAY
#define FEATURE_WHITE_SHADING		DC1394_FEATURE_WHITE_SHADING
#define FEATURE_FRAME_RATE			DC1394_FEATURE_FRAME_RATE
#define FEATURE_ZOOM				DC1394_FEATURE_ZOOM
#define FEATURE_PAN					DC1394_FEATURE_PAN
#define FEATURE_TILT				DC1394_FEATURE_TILT
#define FEATURE_OPTICAL_FILTER		DC1394_FEATURE_OPTICAL_FILTER
#define FEATURE_CAPTURE_SIZE		DC1394_FEATURE_CAPTURE_SIZE
#define FEATURE_CAPTURE_QUALITY		DC1394_FEATURE_CAPTURE_QUALITY
*/
