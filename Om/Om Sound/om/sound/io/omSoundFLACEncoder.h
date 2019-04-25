/*
 * Project:     Om Software
 * Version:     1.0.0
 * Website:     http://www.carlschissler.com/om
 * Author(s):   Carl Schissler
 * 
 * Copyright (c) 2016, Carl Schissler
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 	1. Redistributions of source code must retain the above copyright
 * 	   notice, this list of conditions and the following disclaimer.
 * 	2. Redistributions in binary form must reproduce the above copyright
 * 	   notice, this list of conditions and the following disclaimer in the
 * 	   documentation and/or other materials provided with the distribution.
 * 	3. Neither the name of the copyright holder nor the
 * 	   names of its contributors may be used to endorse or promote products
 * 	   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INCLUDE_OM_SOUND_FLAC_ENCODER_H
#define INCLUDE_OM_SOUND_FLAC_ENCODER_H


#include "omSoundIOConfig.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that handles streaming encoding of the compressed .ogg audio format.
/**
  * This class uses an abstract data stream for output, allowing it to encode
  * .ogg data to a file, network destination, or other destination.
  */
class FLACEncoder : public SoundOutputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a .ogg stream encoder with the given number of channels, bitrate, and sample rate.
			/**
			  * An encoder created by this constructor will write an .ogg file stream with the specified
			  * number of channels and bit rate. The constructor also allows the user to specify
			  * whether or not variable-bitrate encoding is used, the default is yes.
			  * The encoder uses the specified sample rate for all incoming audio, automatically
			  * sample rate converting any audio that doesn't match the output sample rate.
			  */
			FLACEncoder( DataOutputStream* outputStream, Size numChannels, SampleRate newSampleRate,
						const SoundFormatSettings& newFormatSettings = SoundFormatSettings() );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this .ogg encoder and release all associated resources.
			virtual ~FLACEncoder();
			
			
		//********************************************************************************
		//******	Stream Flush Accessor Methods
			
			
			/// Flush all pending sound data to be encoded to the encoder's data output stream.
			virtual void flush();
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not seeking is allowed by this .ogg file encoder.
			virtual Bool canSeek() const;
			
			
			/// Return if this .ogg encoder's current position can be moved by the specified signed sample offset.
			/**
			  * This sample offset is specified as the number of sample frames to move
			  * in the stream - a frame is equal to one sample for each channel in the stream.
			  */
			virtual Bool canSeek( Int64 relativeSampleOffset ) const;
			
			
			/// Move the current sample frame position of the encoder by the specified signed amount.
			/**
			  * This method attempts to seek the position of the encoder by the specified amount.
			  * The method returns the signed amount that the position in the stream was changed
			  * by. Thus, if seeking is not allowed, 0 is returned. Otherwise, the stream should
			  * seek as far as possible in the specified direction and return the actual change
			  * in position.
			  */
			virtual Int64 seek( Int64 relativeSampleOffset );
			
			
		//********************************************************************************
		//******	FLAC File Length Accessor Methods
			
			
			/// Return the total number of samples that have been encoded by this .ogg encoder.
			OM_INLINE SoundSize getLengthInSamples() const
			{
				return lengthInSamples;
			}
			
			
			/// Return the total length of sound in seconds that has been encoded by this .ogg encoder.
			OM_INLINE Double getLengthInSeconds() const
			{
				return lengthInSamples / sampleRate;
			}
			
			
		//********************************************************************************
		//******	Output Format Accessor Methods
			
			
			/// Return the number of channels that are being written by the .ogg encoder.
			/**
			  * This is the number of channels that should be provided to the write() method.
			  * If less than this number of channels is provided, silence is written for the
			  * other channels. Superfluous channels are ignored.
			  *
			  * This value is set in a constructor of the encoder and once set, is read-only.
			  */
			virtual Size getChannelCount() const;
			
			
			/// Return the sample rate at which this .ogg encoder is encoding.
			/**
			  * This is the sample rate to which all incoming sound data is converted to
			  * before being written to the output stream. The output sample rate can be
			  * set in the encoder's constructor. After contruction, the sample rate is
			  * read-only and cannot be changed. If not set during construction, the encoder
			  * can automatically auto-detect the sample rate of the first buffer of output audio
			  * and use that sample rate thenceforth. In that case, this method will return
			  * a sample rate of 0 until the incoming sample rate is detected.
			  */
			virtual SampleRate getSampleRate() const;
			
			
			/// Return the type of sample data that is being written by this .ogg encoder.
			/**
			  * Since FLAC encodes data in frequency domain, this method always returns the
			  * type of SAMPLE_32F.
			  */
			virtual SampleType getNativeSampleType() const;
			
			
		//********************************************************************************
		//******	Encoder Status Accessor Method
			
			
			/// Return whether or not this .ogg encoder is writing a valid FLAC file.
			/**
			  * This method can return FALSE, indicating the encoder cannot be used,
			  * and should be checked by the user to see if the chosen FLAC
			  * format is supported by the encoder. The encoder can also be invalid if the
			  * destination file or stream cannot be opened or is not valid.
			  */
			virtual Bool isValid() const
			{
				return valid;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that wraps any internal ogg encoding state.
			class FLACEncoderWrapper;
			
			
		//********************************************************************************
		//******	Private Copy Operations
			
			
			/// Create a copy of the specified .ogg encoder.
			FLACEncoder( const FLACEncoder& other );

			
			
			/// Assign the current state of another FLACEncoder object to this FLACEncoder object.
			FLACEncoder& operator = ( const FLACEncoder& other );
			
			
		//********************************************************************************
		//******	Private Sound Writing Method
			
			
			/// Write the specified number of samples from the output buffer to the data output stream.
			/**
			  * This method attempts to encode the specified number of samples to the stream
			  * from the output buffer. It then returns the total number of valid samples which
			  * were written to the .ogg file. The current write position in the stream
			  * is advanced by the number of samples that are written.
			  */
			virtual SoundResult writeSamples( const SoundBuffer& outputBuffer, Size numSamples );
			
			
			/// Write the header of the .ogg file, starting at the current position in the data output stream.
			/**
			  * The method returns whether or not there was an error that occurred.
			  * If an error occurs there must be a problem with the output stream format.
			  */
			Bool writeHeader();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to a class containing all ogg decoder internal data.
			FLACEncoderWrapper* wrapper;
			
			
			/// A pointer to the data output stream to which we are encoding .ogg data.
			DataOutputStream* stream;
			
			
			/// A mutex object that provides thread synchronization for this .ogg encoder.
			/**
			  * This thread protects access to encoding parameters such as the current encoding
			  * position so that they are never accessed while audio is being encoded.
			  */
			mutable threads::Mutex encodingMutex;
			
			
			/// An object that handles conversion to the output sample rate if the input is mismatched.
			Resampler resampler;
			
			
			/// A buffer which holds the resulting data after sample rate conversion.
			SoundBuffer sampleRateConversionBuffer;
			
			
			/// The number of channels that are being written by the .ogg encoder.
			Size numChannels;
			
			
			/// The sample rate of the .ogg file that is being encoded.
			SampleRate sampleRate;
			
			
			/// An object containing format settings for this encoder.
			SoundFormatSettings formatSettings;
			
			
			/// The total length of the encoded .ogg file in samples.
			SoundSize lengthInSamples;
			
			
			/// The current position within the .ogg file where the encoder is encoding.
			SampleIndex currentSampleIndex;
			
			
			/// A boolean value indicating whether or not the encoder is currently writing a valid .ogg file.
			Bool valid;
			
			
			/// A boolean value indicating whether or not the encoder has written the .ogg file's header.
			Bool writtenHeader;
			
			
			
};




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FLAC_ENCODER_H
