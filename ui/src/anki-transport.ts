export class AnkiTransport {
  private static readonly url = "http://localhost:8765";

  static async updateLastNote(base64image: string, imageFieldName: string): Promise<void> {
    const cardIdPromise = this.getLastCardId();
    const updateImagePromise = this.uploadImage(base64image);

    const [lastCardId, filename] = await Promise.all([cardIdPromise, updateImagePromise]);
    const imgHTML = `<img src="${filename}" />`;
    const updateFields: Record<string, string> = {};
    updateFields[imageFieldName] = imgHTML;

    const updateNote = {
      action: "updateNoteFields",
      version: 6,
      params: {
        note: {
          id: lastCardId,
          fields: updateFields,
        }
      }
    };

    await fetch(this.url, {
      method: "POST",
      body: JSON.stringify(updateNote),
    });

  }

  private static async getLastCardId(): Promise<number> {
    const findLastCard = {
      action: "findNotes",
      version: 6,
      params: {
        query: "added:1" // This query finds the most recently added card
      }
    };

    const lastCardId = await fetch(this.url, {
      method: 'POST',
      body: JSON.stringify(findLastCard)
    })
      .then(response => response.json())
      .then(data => data.result[0]);

    return lastCardId;
  }

  private static async uploadImage(base64image: string): Promise<string> {
    const imageExt: string = base64image.includes("png") ? "png" : "jpg";
    const filename: string = `${Date.now()}.${imageExt}`;

    const data = base64image.split("base64,")[1];

    const storeMediaFile = {
      action: "storeMediaFile",
      version: 6,
      params: {
        filename,
        data
      }
    };

    await fetch(this.url, { method: "POST", body: JSON.stringify(storeMediaFile) });


    return filename;
  }
};
