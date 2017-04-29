#ifndef PROTOCOL_OBJECT_H_
#define PROTOCOL_OBJECT_H_

namespace protocol
{
	class Object
	{
		public:
			virtual ~Object();

			virtual void serializeRead(class ReadStream& stream) = 0;

			virtual void serializeWrite(class ReadStream& stream) = 0;

			virtual void serializeMeasure(class MeasureStream& stream) = 0;
	};
}


#define PROTOCOL_SERIALIZE_OBJECT(stream)												\
	void serializeRead(class protocol::ReadStream& stream){	Serialize(stream);};		\
	void serializeWrite(class protocol::WriteStram& stream)	{Serialize(stream);};		\
	void SerializeMeasure(class protocol::MeasureStream& stream) {Serialize(stream);};	\
	template <typename Stream> void Serialize(Stream& stream)

#endif