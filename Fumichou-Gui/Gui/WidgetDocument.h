#pragma once

namespace Gui
{
	namespace Document
	{
		struct HeaderText
		{
			String text;
		};

		struct PlainText
		{
			String text;
		};

		struct SplitLine
		{
		};

		struct Drawer
		{
			Vec2 leftCenter;
			int rightmost;

			RectF LineRect() const;

			void operator()(std::monostate) const { return; }
			void operator()(const HeaderText& text) const;
			void operator()(const PlainText& text) const;
			void operator()(SplitLine) const;
		};
	}

	class IDocumentData
	{
	public:
		virtual ~IDocumentData() = default;
		virtual size_t Size() = 0;
		virtual void Draw(int index, const Document::Drawer& drawer) = 0;
	};

	template <typename Drawer = Document::Drawer, typename... T>
	class DocumentData : public IDocumentData
	{
		static_assert(std::is_base_of<Document::Drawer, Drawer>::value);
		static_assert(sizeof(Drawer) == sizeof(Document::Drawer));

	public:
		using data_type = std::variant<
			std::monostate,
			Document::HeaderText,
			Document::PlainText,
			Document::SplitLine,
			T...>;
		using array_type = Array<data_type>;

		~DocumentData() override = default;
		size_t Size() override { return m_data.size(); }

		void Draw(int index, const Document::Drawer& drawer) override
		{
			std::visit(static_cast<const Drawer&>(drawer), m_data[index]);
		}

		array_type& Raw() { return m_data; }
		const array_type& Raw() const { return m_data; }

	private:
		array_type m_data{};
	};

	class WidgetDocument0
	{
	public:
		void Update(const Size& availableRegion);
		IDocumentData& Data();
		const IDocumentData& Data() const;

	protected:
		WidgetDocument0(std::unique_ptr<IDocumentData>&& data);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};

	template <typename DataType>
	class WidgetDocument final : public WidgetDocument0
	{
	public:
		WidgetDocument() : WidgetDocument0(std::make_unique<DataType>())
		{
		}

		DataType& Data()
		{
			return dynamic_cast<DataType&>(WidgetDocument0::Data());
		}

		const DataType& Data() const
		{
			return dynamic_cast<const DataType&>(WidgetDocument0::Data());
		}
	};
}
