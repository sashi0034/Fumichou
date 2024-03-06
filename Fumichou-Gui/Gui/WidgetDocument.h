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

			// 描画の戻り値は領域行数
			int operator()(std::monostate) const { return 1; }
			int operator()(const HeaderText& text) const;
			int operator()(const PlainText& text) const;
			int operator()(SplitLine) const;
		};
	}

	class IDocumentData
	{
	public:
		virtual ~IDocumentData() = default;
		virtual size_t Size() = 0;
		virtual bool IsEmpty(int index) const = 0;
		virtual void InsertEmpty(int index) = 0;
		virtual void Remove(int index) = 0;
		virtual int Draw(int index, const Document::Drawer& drawer) = 0;
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

		bool IsEmpty(int index) const override
		{
			if (index >= m_data.size()) return false;
			return m_data[index].index() == 0;
		}

		void InsertEmpty(int index) override
		{
			m_data.insert(m_data.begin() + index, std::monostate());
		}

		void Remove(int index) override
		{
			m_data.remove_at(index);
		}

		int Draw(int index, const Document::Drawer& drawer) override
		{
			return std::visit(static_cast<const Drawer&>(drawer), m_data[index]);
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
